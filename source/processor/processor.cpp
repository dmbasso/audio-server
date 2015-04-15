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

} //end namespace processor
} //end namespace aserver
