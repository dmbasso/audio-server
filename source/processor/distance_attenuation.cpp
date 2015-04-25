#include "distance_attenuation.h"

#include <iostream>

using namespace std;

namespace aserver {
namespace processor {

void DistanceAttenuation::config(ConfigData *configData)
{
    DistanceAttenuationConfigData *cfgData = (DistanceAttenuationConfigData *) configData;

    motionSamplingRate = cfgData->motionSamplingRate;
    inputListenerPositions = cfgData->inputListenerPositions;
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
    loadListenerPositions();

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
        if(!locations.empty() && locations.begin()->first == i) { // process a moving source
            src->setLocation(locations.begin()->second);
            locations.erase(locations.begin());
        }
        distance = distanceToListener(src->getLocation(), i);
        attenuation = (1. / (distance + 1.));
        src->getGenerator()->buffer->readFrame(sams, i);
        sams[0] *= attenuation;
        sams[1] *= attenuation;
        buffer->mixFrame(sams, i);
    }
}

float DistanceAttenuation::distanceToListener(Location sourceLocation, unsigned frame)
{
    for (auto it : listenerPositions) {
        if (it.first == frame) {
            listenerPosition = it.second;
        }
    }
    return sourceLocation.distanceTo(listenerPosition);
}

void DistanceAttenuation::loadListenerPositions()
{
    float motionSamplingStep = 44100./motionSamplingRate;

    if (inputListenerPositions.size() == 0) {
        return;
    }

    listenerPositions.clear();

    unsigned i;

    for (i = 0; (i*motionSamplingStep + periodicPositionRemainder) < buffer->getPeriodSize(); i++) {
        if (inputListenerPositions.size() == 0) {
            break;
        }
        listenerPositions[round(motionSamplingStep * i + periodicPositionRemainder)] = inputListenerPositions[0];
        inputListenerPositions.erase(inputListenerPositions.begin());
    }
    periodicPositionRemainder = (i * motionSamplingStep + periodicPositionRemainder) - buffer->getPeriodSize();
    listenerPosition = listenerPositions.rbegin()->second;
}

} //end namespace processor
} //end namespace aserver