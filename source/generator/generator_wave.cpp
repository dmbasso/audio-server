#include "generator.h"
#include <iostream>
#include "generator_wave.h"

using namespace std;

namespace aserver {
namespace generator {

Wave::Wave(unsigned periodSize, SoundBuffer* _wave) : Generator(periodSize)
{
    this->wave = _wave;
    this->position = 0;
}

Wave::~Wave()
{

}

void Wave::config(const ConfigData *configData)
{

}

void Wave::render()
{
    int16_t sams[2];

    for (unsigned i = 0; i < buffer->getPeriodSize(); i++, this->position++) {
        if (this->position == wave->getPeriodSize()) { //currently looping all waves
            this->position = 0;
        }
        buffer->writeFrame(wave->readFrame(sams, this->position), i);
    }
}

} //end generator namespace
} //end aserver namespace
