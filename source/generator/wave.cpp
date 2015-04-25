#include "generator.h"
#include <iostream>
#include "wave.h"
#include "../wav_file.h"

using namespace std;

namespace aserver {
namespace generator {

Wave::Wave(unsigned periodSize) : Generator(periodSize)
{
    position = 0;
    increment = 1;
}

Wave::~Wave()
{

}

void Wave::config(const ConfigData *configData)
{
    WaveConfigData* wcd = (WaveConfigData*) configData;
    wave = loadWave(wcd->filename);
    increment = wcd->increment;
    position = wcd->position;
}

void Wave::render()
{
    int16_t sams[2];

    for (unsigned i = 0; i < buffer->getPeriodSize(); i++, position += increment) {
        if (position == wave->getPeriodSize()) { //currently looping all waves
            position = 0;
        }
        buffer->writeFrame(wave->readFrame(sams, position), i);
    }
}

} //end generator namespace
} //end aserver namespace
