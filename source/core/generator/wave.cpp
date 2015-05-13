#include "wave.h"
#include "../wav_file.h"

#include <iostream>

using namespace std;

namespace aserver {
namespace generator {

Wave::Wave(unsigned periodSize) : Generator(periodSize)
{
    WaveConfigData* cfgData = new WaveConfigData();
    cfgData->flags = waveConfigFlags::WAVE_ALL - waveConfigFlags::WAVE_FILENAME;
    config(cfgData);
}

void Wave::config(const ConfigData *configData)
{
    WaveConfigData *cfgData = (WaveConfigData*) configData;

    if (cfgData->flags & waveConfigFlags::WAVE_FILENAME) {
        ifstream ifs(cfgData->filename);
        if (ifs.good()) {
            wave = loadWave(cfgData->filename);
        }
        else {
            cout << "Error opening input wave file = " << cfgData->filename << endl;
            return;
        }
    }
    if (cfgData->flags & waveConfigFlags::INCREMENT) {
        increment = cfgData->increment;
    }
    if (cfgData->flags & waveConfigFlags::POSITION) {
        position = cfgData->position;
    }
}

void Wave::render()
{
    int16_t sams[2];

    if (!wave) {
        cout << "Wave generator rendering an empty wave sound buffer..." << endl;
        return;
    }

    for (unsigned i = 0; i < buffer->getPeriodSize(); i++, position += increment) {
        if (position == wave->getPeriodSize()) { //currently looping all waves
            position = 0;
        }
        buffer->writeFrame(wave->readFrame(sams, position), i);
    }
}

} //end generator namespace
} //end aserver namespace
