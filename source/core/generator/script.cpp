#include <iostream>
#include "script.h"
#include "../core.h"

using namespace std;

namespace aserver {
namespace generator {

Script::Script(Core *core, uint32_t periodSize) : Wave(core, periodSize)
{
    ScriptConfigData *cfgData = new ScriptConfigData(); // set flags for all data in the constructor
    cfgData->flags = scriptConfigFlags::SCRIPT_ALL ; // configure all data fields in constructor
    config(cfgData);
    scriptPosition = 0;

    //temp: load keyframes for testing:
    loadDefaultKeyframes();

    keyframesIt = keyframes.begin();
}

void Script::config(const ConfigData *configData)
{
    ScriptConfigData *cfgData = (ScriptConfigData *) configData;

    if (cfgData->flags & scriptConfigFlags::SCRIPT_PLAYBACK_CMD) {
        switch (cfgData->playbackCommand) {
            case generator::playbackCommand::PLAY : {
                if (playbackState != generator::playbackState::PLAYING) {
                    playbackState = generator::playbackState::PLAYING;
                    cout << "playback state is now playing" << endl;
                }
                break;
            }
            case generator::playbackCommand::STOP : {
                if (playbackState != generator::playbackState::STOPPED) {
                    playbackState = generator::playbackState::STOPPED;
                    cout << "playback state is now stopped" << endl;
                    wavePosition = 0;
                }
                break;
            }
            case generator::playbackCommand::PAUSE : {
                if (playbackState != generator::playbackState::PAUSED) {
                    playbackState = generator::playbackState::PAUSED;
                    cout << "playback state is now paused" << endl;
                }
                break;
            }
            case generator::playbackCommand::PLAY_LOOP : {
                if (playbackState != generator::playbackState::PLAYING_LOOP) {
                    playbackState = generator::playbackState::PLAYING_LOOP;
                    cout << "playback state is now playing looped" << endl;
                }
                break;
            }
            case generator::playbackCommand::REVERSE : {
                if (playbackState != generator::playbackState::REWINDING) {
                    cout << "playback state is now rewinding" << endl;
                    playbackState = generator::playbackState::REWINDING;
                }
                break;
            }
        }
    }

    Wave::config(configData);
}

void Script::render ()
{
    locs.clear();
    buffer->reset();

    uint32_t startIndex = 0;

    if (keyframesIt != keyframes.end()) {
        for (; msecsToSams(keyframesIt->first) < scriptPosition + buffer->getPeriodSize() && keyframesIt != keyframes.end(); keyframesIt++) {
            // first we render the samples with the previous data settings
            if (startIndex != msecsToSams(keyframesIt->first) - scriptPosition) {
                Wave::renderNFrames(startIndex, msecsToSams(keyframesIt->first) - scriptPosition);
                startIndex = msecsToSams(keyframesIt->first) - scriptPosition;
            }
            //then we load the keyframe info:
            Script::config((ConfigData *) &(keyframesIt->second));
            //load source(generator) location from keyframe
            locs[msecsToSams(keyframesIt->first) - scriptPosition] = Location(keyframesIt->second.location[0],
                                                                       keyframesIt->second.location[1],
                                                                       keyframesIt->second.location[2]);
        }
    }

    // render the remaining samples of this buffer
    if (startIndex != buffer->getPeriodSize()) {
        Wave::renderNFrames(startIndex, buffer->getPeriodSize());
    }
    scriptPosition += buffer->getPeriodSize();
}

void Script::addKeyframe(const Keyframe &kf)
{
    keyframes[kf.start] = kf;
}

uint32_t Script::msecsToSams(uint32_t msecs)
{
    return msecs * core->getSamplingRate() * 0.001;
}

void Script::loadDefaultKeyframes()
{
    Keyframe kf1, kf2, kf3;

    kf1.flags = generator::keyframeConfigFlags::KEYFRAME_ALL;
    strncpy(kf1.filename, "audio/input/espiral_seg.wav", 256);
    kf1.playbackCommand = generator::playbackCommand::PLAY_LOOP;
    kf1.frequencyRatio = 1;
    kf1.location[0] = 0.;
    kf1.location[1] = 1.;
    kf1.location[2] = 0.;
    kf1.start = 0;

    addKeyframe(kf1);

//    kf2.flags = generator::keyframeConfigFlags::KEYFRAME_ALL;
//    strncpy(kf2.filename, "audio/input/espiral_seg.wav", 256);
//    kf2.playbackCommand = generator::playbackCommand::PAUSE;
//    kf2.frequencyRatio = 1;
//    kf2.location[0] = 0.;
//    kf2.location[1] = 1.;
//    kf2.location[2] = 0.;
//    kf2.start = 8000;
//
//    addKeyframe(kf2);

//    kf3.flags = generator::keyframeConfigFlags::KEYFRAME_ALL;
//    strncpy(kf3.filename, "audio/input/espiral_seg.wav", 256);
//    kf3.playbackCommand = generator::playbackCommand::PLAY_LOOP;
//    kf3.frequencyRatio = 1;
//    kf3.location[0] = 0.;
//    kf3.location[1] = 1.;
//    kf3.location[2] = 0.;
//    kf3.start = 10000;
//
//    addKeyframe(kf3);

    // progressively raise the input frequency during the first 20 secs rendered
//    vector<Keyframe> kfs = vector<Keyframe>(100);

//    for (uint32_t i = 0; i < 100; i++) {
//        kfs[i].flags = generator::keyframeConfigFlags::KEYFRAME_ALL;
//        strncpy(kfs[i].filename, "audio/input/espiral.wav", 256);
//        kfs[i].playbackCommand = generator::playbackCommand::PLAY;
//        if (i>49 && i<90) kfs[i].playbackCommand = generator::playbackCommand::STOP;
//        kfs[i].frequencyRatio = i/100.;
//        kfs[i].location[0] = 0.;
//        kfs[i].location[1] = 0;
//        kfs[i].location[2] = 0.;
//        kfs[i].start = i * 200;
//        addKeyframe(kfs[i]);
//    }
//
//    // progressively decrease the distance during the first 20 secs rendered
//    vector<Keyframe> kfs2 = vector<Keyframe>(100);
//
//    for (uint32_t i = 0; i < 20; i++) {
//        kfs2[i].flags = generator::keyframeConfigFlags::KEYFRAME_ALL;
//        strncpy(kfs2[i].filename, "audio/input/espiral.wav", 256);
//        kfs2[i].playbackCommand = generator::playbackCommand::PLAY;
//        if (i>=20) kfs2[i].playbackCommand = generator::playbackCommand::REVERSE;
//        kfs2[i].frequencyRatio = 0.527;
//        kfs2[i].location[0] = 0;
//        kfs2[i].location[1] = 0;
//        kfs2[i].location[2] = 0.;
//        kfs2[i].start = i * 200;
//        addKeyframe(kfs2[i]);
//    }
}

} //end generator namespace
} //end aserver namespace
