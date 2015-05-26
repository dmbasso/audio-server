#include "generator.h"

using namespace std;

namespace aserver {
namespace generator {

Generator::Generator(Core *_core, unsigned periodSize)
{
    core = _core;
    buffer = new SoundBuffer(periodSize);
}

void Generator::setPeriodSize(unsigned periodSize)
{
    buffer = new SoundBuffer(periodSize);
}

} //end generator namespace
} //end aserver namespace
