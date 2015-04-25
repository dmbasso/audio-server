#ifndef GENERATOR_PRIMITIVE_H
#define GENERATOR_PRIMITIVE_H

#include "generator.h"

using namespace std;

namespace aserver{
namespace generator {

/** \brief PrimitiveConfigData sets the configuration parameters for a Primitive generator.
*  These parameters are set with their default values within the Primitive generator constructor.
*  The purpose of this class is to be used if there is need to alter the default parameters.
*/

struct PrimitiveConfigData : ConfigData {
    int amplitude = 32767/5;
    unsigned short frequency = 220;
    unsigned short squareFactor = 10;
    waveformType waveform = waveformType::SINE;
};

/** \brief Generates a primitive waveform.
*
*/

class Primitive :public Generator {
protected:
    float phase;
    float frequency;
    int amplitude;
    int squareFactor;
    waveformType waveform;

public:
    Primitive(unsigned periodSize);
    ~Primitive() {};
    void config(const ConfigData *configData) override;
    void render() override;
    void renderNFrames(unsigned start, unsigned end);
};

} //end generator namespace
} //end aserver namespace

#endif
