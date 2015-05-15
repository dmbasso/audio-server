#ifndef GENERATOR_WAVE_H
#define GENERATOR_WAVE_H

#include "base.h"

using namespace std;

namespace aserver{
namespace generator {

enum waveConfigFlags : uint64_t {
    WAVE_FILENAME = 0x1,
    INCREMENT =     0x2,
    POSITION =      0x4,
    WAVE_ALL =      0x7
};

struct WaveConfigData : ConfigData {
    const char *filename;
    float increment = 1;
    float position = 0;
};

class Wave :public Generator {
protected:
    SoundBuffer *wave; //generator parent class has a buffer
    // playbackState playbackState = playbackState::STOPPED;
    float position;
    float increment; //step size to read buffer -> determines new frequency output

public:
    Wave(Core *core, unsigned periodSize);
    virtual ~Wave() {};
    virtual void config(const ConfigData *configData) override;
    virtual void render() override;
};

} //end generator namespace
} //end aserver namespace

#endif
