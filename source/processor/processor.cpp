#include <cmath>

#include "processor.h"

using namespace std;

namespace aserver {
namespace processor {

Processor::Processor(unsigned period)
{
    this->buffer = new SoundBuffer(period);
    this->listenerPosition = Location(0., 0., 0.);
}

} //end namespace processor
} //end namespace aserver
