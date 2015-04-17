#ifndef GENERATOR_WAVE_H
#define GENERATOR_WAVE_H

#include "generator.h"

#include <string>

using namespace std;

namespace aserver{
namespace generator {


class Wave :public Generator {
private:
    SoundBuffer *wave; //generator parent class has a buffer
    // playbackState playbackState = playbackState::STOPPED;
    float position;
    float increment; //step size to read buffer -> determines new frequency output

public:
    Wave(unsigned periodSize, SoundBuffer* sound);
    virtual ~Wave() override;
    virtual void config(const ConfigData *configData) override;
    virtual void render() override;
};

} //end generator namespace
} //end aserver namespace

#endif
