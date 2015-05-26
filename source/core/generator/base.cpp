#include "generator.h"

using namespace std;

namespace aserver {
namespace generator {

Generator::Generator(Core *_core, uint32_t periodSize)
{
    core = _core;
    buffer = new SoundBuffer(periodSize);
}

void Generator::setPeriodSize(uint32_t periodSize)
{
    buffer = new SoundBuffer(periodSize);
}

} //end generator namespace
} //end aserver namespace
