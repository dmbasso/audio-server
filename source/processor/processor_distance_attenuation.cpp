#include "processor_distance_attenuation.h"

#include <iostream>

using namespace std;

namespace aserver {
namespace processor {

void DistanceAttenuation::config(ConfigData *configData)
{

}

/** \brief Add a Source object to the Processor map.
* This method takes a Generator object to allocate a new Source.
* The new Source object is then added to the processors Source map.
*/

void DistanceAttenuation::addSource(generator::Generator *gen, SourceConfigData *srcData)
{
    auto source = new processor::Source();
    source->setGenerator(gen);

    if (srcData) {
        DistanceAttenuationSourceConfigData *srcConfigData = (DistanceAttenuationSourceConfigData*) srcData;
        source->setLocation(srcConfigData->loc);
    }
    this->sources[sourceCounter++] = source;
}

/** \brief Calculates distance attenuation according to the inverse distance law (used for sound pressure measurements).
*  Calculate distance of each source using its location (the default user position is (0,0,0)
*  Calculate the scalar da = 1/distance
*  Multiply the scalar with every sample in the source rendered buffer.
*/

void DistanceAttenuation::render()
{
    this->buffer->reset();

    for (auto const &it : sources) {
        process(it.second);
    }
}

/** \brief Calculates distance attenuation according to the inverse distance law (used for sound pressure measurements).
*  Calculate distance of each source using its location (the default user position is (0,0,0)
*  Calculate the scalar da = 1/distance
*  Multiply the scalar with every sample in the source rendered buffer.
*/

void DistanceAttenuation::process(Source *src)
{
    float distance;
    float attenuation;
    int16_t sams[2];

    map<unsigned, Location> locations = src->getGenerator()->locs;

    distance = src->getLocation().distanceTo(Location());
    attenuation = (1. / (distance + 1.));

    for (int i = 0; i < buffer->getPeriodSize(); i++) {
        if(!locations.empty() && locations.begin()->first == i) {
            src->setLocation(locations.begin()->second);
            locations.erase(locations.begin());
            distance = src->getLocation().distanceTo(Location());
            attenuation = (1. / (distance + 1.));
            cout << "Distance = " << distance << " Attenuation = " << attenuation << endl;
        }
        src->getGenerator()->buffer->readFrame(sams, i);
        sams[0] *= attenuation;
        sams[1] *= attenuation;
        this->buffer->mixFrame(sams, i);
    }
}

} //end namespace processor
} //end namespace aserver
