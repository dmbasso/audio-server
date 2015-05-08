#include <cmath>

#include "processor.h"

using namespace std;

namespace aserver {
namespace processor {

void Source::config(SourceConfigData *srcData)
{
    loc = srcData->loc;
}

Processor::Processor(unsigned period)
{
    buffer = new SoundBuffer(period);
    listenerPosition = Location(0., 0., 0.);
}

map<unsigned, Location> ProcessorInput::loadListenerPositions()
{
    map<unsigned, Location> listenerPositions;
    float motionSamplingStep = 44100./motionSamplingRate;

    if (inputListenerPositions.size() == 0) {
        return listenerPositions;
    }

    unsigned i;

    for (i = 0; (i*motionSamplingStep + periodicPositionRemainder) < periodSize; i++) {
        if (inputListenerPositions.size() == 0) {
            break;
        }
        listenerPositions[round(motionSamplingStep * i + periodicPositionRemainder)] = inputListenerPositions[0];
        inputListenerPositions.erase(inputListenerPositions.begin());
    }

    if (i) {
        periodicPositionRemainder = (i * motionSamplingStep + periodicPositionRemainder) - periodSize;
    }

    return listenerPositions;
}

} //end namespace processor
} //end namespace aserver
