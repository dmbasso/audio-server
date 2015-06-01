#include "base.h"

using namespace std;

namespace aserver {
namespace output {

Output::Output(Core *_core, uint32_t periodSize)
{
    core = _core;
    silence = new SoundBuffer(periodSize);
}

Output::~Output()
{
    delete silence;
}

void Output::setPeriodSize(uint32_t periodSize)
{
    silence = new SoundBuffer(periodSize);
}

} //end output namespace
} //end aserver namespace
