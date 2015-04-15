#include "generator.h"

#include <cmath>
#include <iostream>

using namespace std;

namespace aserver {
namespace generator {

Generator::Generator(unsigned periodSize)
{
    this->buffer = new SoundBuffer(periodSize);
}

} //end generator namespace
} //end aserver namespace
