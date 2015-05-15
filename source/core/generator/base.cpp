#include "generator.h"

using namespace std;

namespace aserver {
namespace generator {

Generator::Generator(unsigned periodSize)
{
    buffer = new SoundBuffer(periodSize);
}

} //end generator namespace
} //end aserver namespace
