#include <cmath>

#include "processor.h"

using namespace std;

namespace aserver {
namespace processor {

Processor::Processor(unsigned period)
{
    buffer = new SoundBuffer(period);
    listenerPosition = Location(0., 0., 0.);
}

} //end namespace processor
} //end namespace aserver
