#ifndef SCRIPT_H
#define SCRIPT_H

#include "wave.h"

using namespace std;

namespace aserver {
namespace generator {

enum keyframeConfigFlags : uint64_t {
    START =             0x1,
    KEYFRAME_FILENAME = 0x2,
    KEYFRAME_POSITION = 0X4,
    NOTE_RATIO =        0X8,
    KEYFRAME_ALL =      0XF
};

struct Keyframe {
    int flags;
    unsigned start;
    char filename[256];
    float location[3];
    float noteRatio;
};

enum scriptConfigFlags : uint64_t {
    PLAYBACK_STATE =    0x1,
    SCRIPT_ALL =        0x1
};

struct ScriptConfigData : ConfigData {
    generator::playbackState playbackState = generator::playbackState::PLAYING;
};


class Script :public Wave {
    private:
        map<unsigned, Keyframe> keyframes;
        map<unsigned, Keyframe>::iterator keyframesIt;
        generator::playbackState playbackState;
        unsigned counter;
    public:
        Script(Core *core, unsigned periodSize);
        void config(const ConfigData *configData) override;
        void render() override;
        // Temp methods for testing
        void addKeyframe(const Keyframe &kf);
        void loadDefaultKeyframes();
};

} //end generator namespace
} //end aserver namespace

#endif
