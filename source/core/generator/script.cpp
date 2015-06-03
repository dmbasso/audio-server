#include <iostream>
#include "script.h"
#include "../core.h"

using namespace std;

namespace aserver {
namespace generator {

Script::Script(Core *core, uint32_t periodSize) : Wave(core, periodSize)
{
    scriptState = playbackState::STOPPED;
    scriptPosition = 0;
}

void Script::config(const ConfigData *configData)
{
    ScriptConfigData *cfgData = (ScriptConfigData *) configData;

    if ((cfgData->flags & scriptConfigFlags::COMMAND) == 0) {
        return;
    }

    switch (cfgData->command) {
        case scriptCommand::ADD_KEYFRAMES: {
            addKeyframes(cfgData->keyframeCount, cfgData->keyframes);
            break;
        }
        case scriptCommand::RESET_KEYFRAMES: {
            resetKeyframes();
            break;
        }
        case scriptCommand::PLAY: {
            if (!keyframes.size()) {
                cout << "script " << this << " has nothing to play" << endl;
                break;
            }
            scriptPosition = 0;
            keyframesIt = keyframes.begin();
            scriptState = playbackState::PLAYING;
            cout << "script " << this << " started playing from the beginning" << endl;
            break;
        }
        // TODO: fix this section
        case scriptCommand::STOP : {
            keyframesIt = keyframes.begin();
            scriptState = playbackState::STOPPED;
            cout << "script " << this << " stopped" << endl;
            break;
        }
        case scriptCommand::PAUSE : {
            if (!keyframes.size()) {
                cout << "script " << this << " has nothing to play" << endl;
                break;
            }
            scriptState = playbackState::PAUSED;
            cout << "script " << this << " paused " << endl;
            break;
        }
        case scriptCommand::PLAY_LOOP : {
            if (!keyframes.size()) {
                cout << "script " << this << " has nothing to play" << endl;
                break;
            }
            scriptPosition = 0;
            keyframesIt = keyframes.begin();
            scriptState = playbackState::PLAYING;
            cout << "script " << this << " started playing in loop from the beginning" << endl;
            break;
        }
        case scriptCommand::REVERSE : {
            if (state != playbackState::REWINDING) {
                cout << "playback state is now rewinding" << endl;
                state = playbackState::REWINDING;
            }
            break;
        }
    }
}

void Script::render ()
{
    locs.clear();
    buffer->reset();

    uint32_t startIndex = 0;

    switch(scriptState) {
        case playbackState::PLAYING_LOOP:

        case playbackState::PLAYING:
            if (keyframesIt != keyframes.end()) {
                for (; msecsToSams(keyframesIt->first) < scriptPosition + buffer->getPeriodSize() &&
                       keyframesIt != keyframes.end(); keyframesIt++) {
                    // first we render the samples with the previous data settings
                    uint64_t bufferPosition = msecsToSams(keyframesIt->first) - scriptPosition;
                    if (startIndex != bufferPosition) {
                        Wave::renderNFrames(startIndex, bufferPosition);
                        startIndex = bufferPosition;
                    }
                    //then we load the keyframe info:
                    Wave::config(&keyframesIt->second);
                    //load source(generator) location from keyframe
                    locs[bufferPosition] = Location(keyframesIt->second.location[0],
                                                    keyframesIt->second.location[1],
                                                    keyframesIt->second.location[2]);
                }
                if (keyframesIt == keyframes.end()) {
                    scriptState = playbackState::STOPPED;
                }
            }
            // render the remaining samples of this buffer
            if (startIndex != buffer->getPeriodSize()) {
                Wave::renderNFrames(startIndex, buffer->getPeriodSize());
            }
            scriptPosition += buffer->getPeriodSize();
            break;
        case playbackState::PAUSED:

            break;
        case playbackState::STOPPED:
            scriptPosition = 0;
            break;
    }
}

void Script::addKeyframes(uint32_t count, const Keyframe* newKeyframes)
{
    for (int i=0; i<count; i++) {
        auto &kf = newKeyframes[i];
        keyframes[kf.start] = kf;
    }
    cout << count << " keyframes added to script "
         << this << ", current total: " << keyframes.size() << endl;
}

void Script::resetKeyframes()
{
    keyframes.clear();
    scriptState = playbackState::STOPPED;
    cout << "script " << this << " keyframes cleared" << endl;
}

uint32_t Script::msecsToSams(uint32_t msecs)
{
    return msecs * core->getSamplingRate() / 1000;
}

} //end generator namespace
} //end aserver namespace
