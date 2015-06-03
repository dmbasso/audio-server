#ifndef SCRIPT_H
#define SCRIPT_H

#include "wave.h"

using namespace std;

namespace aserver {
namespace generator {

enum keyframeConfigFlags : uint64_t {
    LOCATION = 0x10000,
};

#pragma pack(1)  // force byte alignment

struct Keyframe : WaveConfigData {
    uint64_t start = 0;
    float location[3] = {0., 0., 0.};
};

enum class scriptCommand : uint8_t {
    // copied because we can't derive from playbackCommand:
    PLAY = 1,
    STOP = 2,
    PAUSE = 3,
    PLAY_LOOP = 4,
    REVERSE = 5,
    // script exclusive commands:
    ADD_KEYFRAMES =     100,
    RESET_KEYFRAMES =   101
};

enum scriptConfigFlags : uint64_t {
    COMMAND = 0x1,
};

struct ScriptConfigData : ConfigData {
    scriptCommand command = scriptCommand::STOP;
    uint32_t keyframeCount = 0;
    Keyframe *keyframes = nullptr;
};

#pragma pack()

class Script : public Wave {
    private:
        map<uint64_t, Keyframe> keyframes;
        map<uint64_t, Keyframe>::iterator keyframesIt;
        uint64_t scriptPosition;
        playbackState scriptState;

    public:
        Script(Core *core, uint32_t periodSize);
        void config(const ConfigData *configData) override;
        void render() override;
        void addKeyframes(uint32_t count, const Keyframe* keyframes);
        void resetKeyframes();
        uint32_t msecsToSams(uint32_t msecs);
};

} //end generator namespace
} //end aserver namespace

#endif
