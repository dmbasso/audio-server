#include "generator.h"
#include <iostream>
#include "wave.h"
#include "../wav_file.h"

using namespace std;

namespace aserver {
namespace generator {

Wave::Wave(unsigned periodSize) : Generator(periodSize)
{
    this->position = 0;
    this->increment = 1;
}

Wave::~Wave()
{

}

void Wave::config(const ConfigData *configData)
{
    WaveConfigData* wcd = (WaveConfigData*) configData;
    this->wave = loadWave(wcd->filename);
    this->increment = wcd->increment;
    this->position = wcd->position;
}

void Wave::render()
{
    int16_t sams[2];

    for (unsigned i = 0; i < buffer->getPeriodSize(); i++, this->position+=this->increment) {
        if (this->position == wave->getPeriodSize()) { //currently looping all waves
            this->position = 0;
        }
        buffer->writeFrame(wave->readFrame(sams, this->position), i);
    }
}

} //end generator namespace
} //end aserver namespace
