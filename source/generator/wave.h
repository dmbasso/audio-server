#ifndef GENERATOR_WAVE_H
#define GENERATOR_WAVE_H

#include "generator.h"

#include <string>

using namespace std;

namespace aserver{
namespace generator {

struct WaveConfigData : ConfigData {
    string filename;
    float increment = 1;
    float position = 0;
};

class Wave :public Generator {
private:
    SoundBuffer *wave; //generator parent class has a buffer
    // playbackState playbackState = playbackState::STOPPED;
    float position;
    float increment; //step size to read buffer -> determines new frequency output

public:
    Wave(unsigned periodSize);
    virtual ~Wave() override;
    virtual void config(const ConfigData *configData) override;
    virtual void render() override;
};

} //end generator namespace
} //end aserver namespace

#endif
