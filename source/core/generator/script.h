#ifndef SCRIPT_H
#define SCRIPT_H

#include "wave.h"

using namespace std;

namespace aserver {
namespace generator {

enum scriptConfigFlags : uint64_t {
    SCRIPT_PLAYBACK_CMD =   0x10000,
    SCRIPT_ALL =            0x10000
};

struct ScriptConfigData : WaveConfigData {
    generator::playbackCommand playbackCommand = generator::playbackCommand::STOP;
};

enum keyframeConfigFlags : uint64_t {
    START =             0x100000000,
    KEYFRAME_FILENAME =         0x1,
    KEYFRAME_LOCATION = 0x200000000,
    NOTE_RATIO =                0x2,
    KEYFRAME_PLAYBACK_CMD = 0x10000,
    KEYFRAME_ALL =      0X300010003
};

struct Keyframe : ScriptConfigData {
    uint32_t start = 0;
    float location[3] = {0., 0., 0.};
};

class Script :public Wave {
    private:
        map<uint32_t, Keyframe> keyframes;
        map<uint32_t, Keyframe>::iterator keyframesIt;
        uint32_t scriptPosition;
    public:
        Script(Core *core, uint32_t periodSize);
        void config(const ConfigData *configData) override;
        void render() override;
        void addKeyframe(const Keyframe &kf);
        uint32_t msecsToSams(uint32_t msecs);

        // Temp methods for testing
        void loadDefaultKeyframes();
};

} //end generator namespace
} //end aserver namespace

#endif
