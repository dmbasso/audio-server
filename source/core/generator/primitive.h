#ifndef GENERATOR_PRIMITIVE_H
#define GENERATOR_PRIMITIVE_H

#include "base.h"
#include <climits>

using namespace std;

namespace aserver{
namespace generator {

/** \brief PrimitiveConfigData sets the configuration parameters for a Primitive generator.
*  These parameters are set with their default values within the Primitive generator constructor.
*  The purpose of this class is to be used if there is need to alter the default parameters.
*  2 bytes of uint64_t flags are reserved for primitive configuration
*/

enum primitiveConfigFlags : uint64_t{
    AMPLITUDE = 0x1,
    FREQUENCY = 0x2,
    SQUARE_FACTOR = 0x4,
    WAVEFORM = 0x8,
    PRIMITIVE_ALL = 0xF
};

struct PrimitiveConfigData : ConfigData {
    unsigned amplitude = SHRT_MAX * 0.2;
    unsigned short frequency = 220;
    unsigned short squareFactor = 10;
    waveformType waveform = waveformType::SINE;
};

/** \brief Generates a primitive waveform.
*
*/

class Primitive :public Generator {
    protected:
        float phase = 1.;
        unsigned amplitude;
        unsigned short frequency;
        unsigned short squareFactor;
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
