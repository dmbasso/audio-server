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
    const char *filename;
    float position[3] = {0,0,0};
    float noteRatio;
};

enum scriptConfigFlags : uint64_t {
    PLAYBACK_STATE =    0x1,
    SCRIPT_ALL =        0x1
};

struct ScriptConfigData : WaveConfigData {
    generator::playbackState playbackState = generator::playbackState::STOPPED;
};


class Script :public Wave {
    private:
        map<unsigned, Keyframe> keyframes;
        generator::playbackState playbackState;
        unsigned counter;
    public:
        Script(Core *core, unsigned periodSize);
        void config(const ConfigData *configData) override;
        void addKeyframe(const Keyframe kf);
        void render() override;
};

} //end generator namespace
} //end aserver namespace

#endif