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
    cfgData->flags = waveConfigFlags::WAVE_ALL - waveConfigFlags::WAVE_INDEX;
    config(cfgData);
}

void Wave::config(const ConfigData *configData)
{
    WaveConfigData *cfgData = (WaveConfigData*) configData;

    if (cfgData->flags & waveConfigFlags::WAVE_INDEX) {
        wave = core->getWave(cfgData->waveIndex);
        wavePosition = 0;
    }
    if (cfgData->flags & waveConfigFlags::FREQUENCY_RATIO) {
        frequencyRatio = cfgData->frequencyRatio;
    }
    if (cfgData->flags & waveConfigFlags::POSITION) {
        wavePosition = cfgData->wavePosition;
    }
    if (cfgData->flags & waveConfigFlags::PLAYBACK_COMMAND) {
        performCommand((playbackCommand) cfgData->command);
    }

    cout << "configuring wave " << wave << " " <<  (int)cfgData->command << "\n";
}

void Wave::performCommand(playbackCommand command)
{
    cout << "performCommand " << (int)command << endl;
    switch (command) {
        case playbackCommand::PLAY: {
            state = playbackState::PLAYING;
            cout << "playback state is now playing" << endl;
            break;
        }
        case playbackCommand::STOP : {
            state = playbackState::STOPPED;
            cout << "playback state is now stopped" << endl;
            wavePosition = 0;
            break;
        }
        case playbackCommand::PAUSE : {
            state = playbackState::PAUSED;
            cout << "playback state is now paused" << endl;
            break;
        }
        case playbackCommand::PLAY_LOOP : {
            state = playbackState::PLAYING_LOOP;
            cout << "playback state is now playing looped" << endl;
            break;
        }
        case playbackCommand::REVERSE : {
            cout << "playback state is now rewinding" << endl;
            state = playbackState::REWINDING;
            break;
        }
    }
}

void Wave::renderNFrames(uint32_t start, uint32_t end)
{
    int16_t sams[2];

    if (!wave) {
        cout << "Wave sound buffer is empty, rendering silence..." << endl;
        return;
    }

    switch (state) {
        case playbackState::PLAYING: {
            if (wavePosition < wave->getPeriodSize()) {
                for (uint32_t i = start; i < end; i++, wavePosition += frequencyRatio) {
                    if (wavePosition >= wave->getPeriodSize()) {
                        break;
                    }
                    buffer->writeFrame(wave->readFrame(sams, wavePosition), i);
                }
            }
            break;
        }
        case playbackState::PLAYING_LOOP: {
            for (uint32_t i = start; i < end; i++, wavePosition += frequencyRatio) {
                if (wavePosition >= wave->getPeriodSize()) {
                    wavePosition = 0;
                }
                buffer->writeFrame(wave->readFrame(sams, wavePosition), i);
            }
            break;
        }
        case playbackState::STOPPED: {

            break;
        }
        case playbackState::PAUSED: {

            break;
        }
        case playbackState::REWINDING: {
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
