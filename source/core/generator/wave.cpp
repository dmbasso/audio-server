#include "wave.h"
#include "../wav_file.h"
#include "../core.h"

#include <iostream>

using namespace std;

namespace aserver {
namespace generator {

Wave::Wave(Core *core, uint32_t periodSize) : Generator(core, periodSize)
{
    WaveConfigData* cfgData = new WaveConfigData();
    cfgData->flags = waveConfigFlags::WAVE_ALL - waveConfigFlags::WAVE_FILENAME;
    config(cfgData);
}

void Wave::config(const ConfigData *configData)
{
    WaveConfigData *cfgData = (WaveConfigData*) configData;

    if (cfgData->flags & waveConfigFlags::WAVE_FILENAME) {
        if (wave != core->getWave(cfgData->filename)) {
            ifstream ifs(cfgData->filename);
            if (ifs.good()) {
                wave = core->getWave(cfgData->filename);
                wavePosition = 0;
            }
            else {
                cout << "Error opening input wave file = " << cfgData->filename << endl;
                return;
            }
        }
    }
    if (cfgData->flags & waveConfigFlags::FREQUENCY_RATIO) {
        frequencyRatio = cfgData->frequencyRatio;
    }
    if (cfgData->flags & waveConfigFlags::POSITION) {
        wavePosition = cfgData->wavePosition;
    }
}

void Wave::renderNFrames(uint32_t start, uint32_t end)
{
    int16_t sams[2];

    if (!wave) {
        cout << "Wave sound buffer is empty, rendering silence..." << endl;
        return;
    }

    switch (playbackState) {
        case generator::playbackState::PLAYING : {
            if (wavePosition < wave->getPeriodSize()) {
                for (uint32_t i = start; i < end; i++, wavePosition += frequencyRatio) {
                    if (wavePosition == wave->getPeriodSize()) {
                        break;
                    }
                    buffer->writeFrame(wave->readFrame(sams, wavePosition), i);
                }
            }
            break;
        }
        case generator::playbackState::PLAYING_LOOP : {
            for (uint32_t i = start; i < end; i++, wavePosition += frequencyRatio) {
                if (wavePosition == wave->getPeriodSize()) {
                    wavePosition = 0;
                }
                buffer->writeFrame(wave->readFrame(sams, wavePosition), i);
            }
            break;
        }
        case generator::playbackState::STOPPED : {

            break;
        }
        case generator::playbackState::PAUSED : {

            break;
        }
        case generator::playbackState::REWINDING : {
            //rewinding stops when the start of the sound is reached
            if (wavePosition > 0) {
                for (uint32_t i = start; i < end; i++, wavePosition -= frequencyRatio) {
                    if (wavePosition <= 0) {
                        wavePosition = 0;
                        break;
                    }
                    buffer->writeFrame(wave->readFrame(sams, wavePosition), i);
                }
            }
            break;
        }
    }
}

void Wave::render()
{
    renderNFrames(0, buffer->getPeriodSize());
}

} //end generator namespace
} //end aserver namespace
