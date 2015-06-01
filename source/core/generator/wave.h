#ifndef GENERATOR_WAVE_H
#define GENERATOR_WAVE_H

#include "base.h"

using namespace std;

namespace aserver{
namespace generator {

enum waveConfigFlags : uint64_t {
    WAVE_INDEX =        0x1,
    FREQUENCY_RATIO =   0x2,
    POSITION =          0x4,
    PLAYBACK_COMMAND =  0x8,
    WAVE_ALL =          0xf
};

#pragma pack(1)  // force byte-alignment

struct WaveConfigData : ConfigData {
    playbackCommand command = playbackCommand::STOP;
    int16_t waveIndex = -1;
    float frequencyRatio = 1;
    float wavePosition = 0;
};

#pragma pack()

class Wave :public Generator {
protected:
    SoundBuffer *wave; //generator parent class has a buffer
    playbackState state;
    float wavePosition;
    float frequencyRatio; //step size to read buffer -> determines new frequency output

    void performCommand(playbackCommand command);

public:
    Wave(Core *core, uint32_t periodSize);
    virtual void config(const ConfigData *configData) override;
    virtual void render() override;
    void renderNFrames(uint32_t start, uint32_t end);
};

} //end generator namespace
} //end aserver namespace

#endif
