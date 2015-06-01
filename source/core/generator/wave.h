#ifndef GENERATOR_WAVE_H
#define GENERATOR_WAVE_H

#include "base.h"

using namespace std;

namespace aserver{
namespace generator {

enum waveConfigFlags : uint64_t {
    WAVE_FILENAME =     0x1,
    FREQUENCY_RATIO =   0x2,
    POSITION =          0x4,
    WAVE_ALL =          0x7
};

#pragma pack(1)  // force byte-alignment

struct WaveConfigData : ConfigData {
    char filename[256] = {0};
    float frequencyRatio = 1;
    float wavePosition = 0;
};

#pragma pack()

class Wave :public Generator {
protected:
    SoundBuffer *wave; //generator parent class has a buffer
    generator::playbackState playbackState;
    float wavePosition;
    float frequencyRatio; //step size to read buffer -> determines new frequency output

public:
    Wave(Core *core, uint32_t periodSize);
    virtual void config(const ConfigData *configData) override;
    virtual void render() override;
    void renderNFrames(uint32_t start, uint32_t end);
};

} //end generator namespace
} //end aserver namespace

#endif
