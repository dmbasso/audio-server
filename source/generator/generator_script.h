#ifndef GENERATOR_SCRIPT_H
#define GENERATOR_SCRIPT_H

#include "generator.h"
#include "generator_wave.h"

using namespace std;

namespace aserver {
namespace generator {

class Script :public Wave {
private:
    //keyframe and other info
public:
    virtual void render() override;
};

} //end generator namespace
} //end aserver namespace

#endif