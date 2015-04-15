#include <cmath>
#include <iostream>

#include "processor.h"

using namespace std;

namespace aserver {
namespace processor {

Processor::Processor(unsigned period)
{
    buffer = new SoundBuffer(period);
}

void NoOperation::config(ConfigData *configData)
{

}

/** \brief Add a Source object to the Processor map.
 * This method takes a Generator object to allocate a new Source.
 * The new Source object is then added to the processors Source map.
 */

    //\todo update map insertion

void NoOperation::addSource(generator::Generator *gen, SourceConfigData *srcData)
{
    auto source = new processor::Source();
    source->setGenerator(gen);

    if (srcData) {
        DistanceAttenuationSourceConfigData *srcConfigData = (DistanceAttenuationSourceConfigData*) srcData;
        source->setLocation(srcConfigData->loc);
    }

    this->sources.insert(std::pair<int, Source*>(sourceCounter++,source));
}

    //\todo loop the Core generators
    //\todo check this method with new mixFrame methods

void NoOperation::render()
{
    //Should we iterate the generators in the processors source map or the core generators?
    //Currently iterating the processor sources..

    this->buffer->reset();

    for (auto const &it : sources) {
        auto gen = it.second->getGenerator();
        gen->render();
    }
}

void Acousticave::config(ConfigData *configData)
{
    //auto cfg = (AcousticaveConfigData*) configData;
    //if (cfg->flags & blabla::setReverb) {
    //    bla bla bla
    //}
}

void Acousticave::addSource(generator::Generator *gen, SourceConfigData *srcData)
{
    //auto source = new processor::AcousticaveSource
    //insert it
}

void Acousticave::render()
{
    //do whatever Acousticave needs to be done,
    //then copy the results into `buffer`
}

void DistanceAttenuation::config(ConfigData *configData)
{
    //auto cfg = (AcousticaveConfigData*) configData;
    //if (cfg->flags & blabla::setReverb) {
    //    bla bla bla
    //}
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
