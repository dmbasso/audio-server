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
    counter = 0;

    //temp: load keyframes for testing:
    loadDefaultKeyframes();

    keyframesIt = keyframes.begin();
}

void Script::config(const ConfigData *configData)
{
    ScriptConfigData *cfgData = (ScriptConfigData *) configData;

    if (cfgData->flags & scriptConfigFlags::PLAYBACK_STATE) {
        playbackState = cfgData->playbackState;
    }
}

void Script::render ()
{
    locs.clear();
    buffer->reset();

    switch (playbackState) {
        case generator::playbackState::PLAYING : {
            //manage previous state
            if (playbackState != generator::playbackState::PLAYING) {
                playbackState = generator::playbackState::PLAYING;
            }
            renderPlaying();
            break;
        }
        case generator::playbackState::STOPPED : {

            cout << "Script generator state = STOPPED" << endl;

            //manage previous state
            if (playbackState != generator::playbackState::STOPPED) {
                playbackState = generator::playbackState::STOPPED;
                counter = 0;
            }

            break;
        }
        case generator::playbackState::PAUSED : {

            cout << "Script generator state = PAUSED" << endl;

            break;
        }
        case generator::playbackState::REWINDING : {

            cout << "Script generator state = REWINDING" << endl;

            break;
        }
    }

}

void Script::renderPlaying()
{
    uint32_t startIndex = 0;

    if (keyframesIt != keyframes.end()) {
        for (; msecsToSams(keyframesIt->first) < counter + buffer->getPeriodSize() && keyframesIt != keyframes.end(); keyframesIt++) {
            // first we render the samples with the previous data settings
            Wave::renderNFrames(startIndex, msecsToSams(keyframesIt->first) - counter);
            startIndex = msecsToSams(keyframesIt->first) - counter;

            //then we load the keyframe info:
            Wave::config((ConfigData *) &(keyframesIt->second));
            //load source(generator) position from keyframe
            locs[msecsToSams(keyframesIt->first) - counter] = Location(keyframesIt->second.location[0],
                                                          keyframesIt->second.location[1],
                                                          keyframesIt->second.location[2]);
        }
    }

    // render the remaining samples of this buffer
    if (startIndex != buffer->getPeriodSize()) {
        Wave::renderNFrames(startIndex, buffer->getPeriodSize());
    }
    counter += buffer->getPeriodSize();
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
    strncpy(kf1.filename, "audio/input/Olson.wav", 256);
    kf1.increment = 1;
    kf1.location[0] = 0.;
    kf1.location[1] = 0.;
    kf1.location[2] = 0.;
    kf1.start = 0;

    addKeyframe(kf1);

    kf2.flags = generator::keyframeConfigFlags::KEYFRAME_ALL;
    strncpy(kf2.filename, "audio/input/espiral.wav", 256);
    kf2.increment = 1.5;
    kf2.location[0] = 0.;
    kf2.location[1] = 0.;
    kf2.location[2] = 0.;
    kf2.start = 2000;

    addKeyframe(kf2);

    kf3.flags = generator::keyframeConfigFlags::KEYFRAME_ALL;
    strncpy(kf3.filename, "audio/input/harp.wav", 256);
    kf3.increment = 1.;
    kf3.location[0] = 0.;
    kf3.location[1] = 0.;
    kf3.location[2] = 0.;
    kf3.start = 10000;

    addKeyframe(kf3);

    // progressively raise the input frequency during the first 20 secs rendered
//    vector<Keyframe> kfs = vector<Keyframe>(100);
//
//    for (uint32_t i = 0; i < 100; i++) {
//        kfs[i].flags = generator::keyframeConfigFlags::KEYFRAME_ALL;
//        strncpy(kfs[i].filename, "audio/input/espiral.wav", 256);
//        kfs[i].increment = i/100.;
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
//    for (uint32_t i = 0; i < 100; i++) {
//        kfs2[i].flags = generator::keyframeConfigFlags::KEYFRAME_ALL;
//        strncpy(kfs2[i].filename, "../audio/input/espiral.wav", 256);
//        kfs2[i].increment = i/100.;
//        kfs2[i].location[0] = 100./i;
//        kfs2[i].location[1] = 0;
//        kfs2[i].location[2] = 0.;
//        kfs2[i].start = i * 200;
//        addKeyframe(kfs2[i]);
//    }
}

} //end generator namespace
} //end aserver namespace
