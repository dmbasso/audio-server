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
    PRIMITIVE_AMPLITUDE =   0x1,
    FREQUENCY =             0x2,
    SQUARE_FACTOR =         0x4,
    WAVEFORM =              0x8,
    PRIMITIVE_ALL =         0xF
};

#pragma pack(1)  // force byte-alignment

struct PrimitiveConfigData : ConfigData {
    uint32_t amplitude = SHRT_MAX;
    uint16_t frequency = 220;
    uint16_t squareFactor = 10;
    waveformType waveform = waveformType::SINE;
};

#pragma pack()

/** \brief Generates a primitive waveform.
*
*/

class Primitive :public Generator {
    protected:
        double phase = 1.;
        uint32_t amplitude;
        uint16_t frequency;
        uint16_t squareFactor;
        waveformType waveform;

    public:
        Primitive(Core *core, uint32_t periodSize);
        ~Primitive() {};
        void config(const ConfigData *configData) override;
        void render() override;
        void renderNFrames(uint32_t start, uint32_t end);
};

} //end generator namespace
} //end aserver namespace

#endif
