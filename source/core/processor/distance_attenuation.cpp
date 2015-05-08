#include "distance_attenuation.h"

#include <iostream>

using namespace std;

namespace aserver {
namespace processor {

void DistanceAttenuation::config(ConfigData *configData)
{
    DistanceAttenuationConfigData *cfgData = (DistanceAttenuationConfigData *) configData;

    input = cfgData->input;
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
        source->setLocation(srcData->loc);
    }
    sources[sourceCounter++] = source;
}

/** \brief Calculates distance attenuation according to the inverse distance law (used for sound pressure measurements).
*  Calculate distance of each source using its location (the default user position is (0,0,0)
*  Calculate the scalar da = 1/distance
*  Multiply the scalar with every sample in the source rendered buffer.
*/

void DistanceAttenuation::render()
{
    buffer->reset();

    // update listener position:
    listenerPositions = input.loadListenerPositions();
    if (!listenerPositions.empty()) {
        listenerPosition = listenerPositions.begin()->second;
    }

    for (auto const &it : sources) {
        process(it.second);
    }

    listenerPositions.clear();
}

/** \brief Calculates distance attenuation according to the inverse distance law (used for sound pressure measurements).
*  Calculate distance of each source using its location (the default user position is (0,0,0)
*  Calculate the scalar da = 1/distance
*  Multiply the scalar with every sample in the source rendered buffer.
*/

void DistanceAttenuation::process(Source *src)
{
    float distance, attenuation;
    int16_t sams[2];

    map<unsigned, Location>::iterator sourceLocationsIt = src->getGenerator()->locs.begin();
    map<unsigned, Location>::iterator listenerLocationsIt = listenerPositions.begin();

    for (int i = 0; i < buffer->getPeriodSize(); i++) {
        if(!src->getGenerator()->locs.empty() && sourceLocationsIt->first == i) { // process a moving source
            src->setLocation(sourceLocationsIt->second);
            sourceLocationsIt++;
        }

        if(!listenerPositions.empty() && listenerLocationsIt->first == i) { // process a moving listener
            listenerPosition = listenerLocationsIt->second;
            listenerLocationsIt++;
        }

        distance = src->getLocation().distanceTo(listenerPosition);
        attenuation = (1. / (distance + 1.));

        src->getGenerator()->buffer->readFrame(sams, i);
        sams[0] *= attenuation;
        sams[1] *= attenuation;
        buffer->mixFrame(sams, i);
    }
}

} //end namespace processor
} //end namespace aserver