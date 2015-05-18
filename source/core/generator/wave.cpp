#include "wave.h"
#include "../wav_file.h"
#include "../core.h"

#include <iostream>

using namespace std;

namespace aserver {
namespace generator {

Wave::Wave(Core *core, unsigned periodSize) : Generator(core, periodSize)
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
            wave = core->getWave(cfgData->filename);
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

void Wave::renderNFrames(unsigned start, unsigned end)
{
    int16_t sams[2];

    if (!wave) {
        cout << "Wave sound buffer is empty, rendering silence..." << endl;
        return;
    }

    for (unsigned i = start; i < end; i++, position += increment) {
        if (position == wave->getPeriodSize()) { //currently looping all waves
            position = 0;
        }
        buffer->writeFrame(wave->readFrame(sams, position), i);
    }
}

void Wave::render()
{
    renderNFrames(0, buffer->getPeriodSize());
}

} //end generator namespace
} //end aserver namespace
