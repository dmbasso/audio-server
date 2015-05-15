#include "script.h"
#include "../core.h"

using namespace std;

namespace aserver {
namespace generator {

Script::Script(Core *core, unsigned periodSize) : Wave(core, periodSize)
{
    ScriptConfigData *cfgData = new ScriptConfigData(); // set flags for all data in the constructor
    cfgData->flags = scriptConfigFlags::SCRIPT_ALL ; // configure all data fields in constructor
    config(cfgData);
    counter = 0;
}

void Script::config(const ConfigData *configData)
{
    ScriptConfigData *cfgData = (ScriptConfigData *) configData;

    if (cfgData->flags & scriptConfigFlags::PLAYBACK_STATE) {
        playbackState = cfgData->playbackState;
    }
}

void Script::addKeyframe(const Keyframe kf)
{
    keyframes[kf.start] = kf;
}

void Script::render ()
{
    switch (playbackState) {
        case generator::playbackState::PLAYING : {

            //manage previous state
            if (playbackState == generator::playbackState::STOPPED) {
                counter = 0;
            }

            playbackState = generator::playbackState::PLAYING;

            map<unsigned, Keyframe>::iterator keyframesIt;// = keyframes.begin();

            for (unsigned i = 0; i < keyframes.size(); i++) {
                if (keyframesIt->first >= counter && keyframesIt->first < counter + buffer->getPeriodSize()) {
                    break;
                }
                else {
                    keyframesIt++;
                }
            }

            int16_t sams[2];

            for (unsigned i = counter; i < buffer->getPeriodSize() + counter; i++) {
                if (i == keyframesIt->first) {
                    //load keyframe info:
                    wave = core->getWave(keyframesIt->second.filename);
                    increment = keyframesIt->second.noteRatio;
                    //load source(generator) position from keyframe

                    position = 0; //start wave at the beginning
                    keyframesIt++; // go to the next keyframe
                }
                //Same as Wave::render();
                if (position == wave->getPeriodSize()) { //currently looping all waves
                    position = 0;
                }
                buffer->writeFrame(wave->readFrame(sams, position), i);
            }
            counter += buffer->getPeriodSize();
            break;
        }
        case generator::playbackState::STOPPED : {

            //manage previous state
            if (playbackState == generator::playbackState::PLAYING) {
                counter = 0;
            }

            playbackState = generator::playbackState::STOPPED;

            break;
        }
        case generator::playbackState::PAUSED : {

            break;
        }
        case generator::playbackState::REWINDING : {

            break;
        }
    }

}

} //end generator namespace
} //end aserver namespace