#include "primitive.h"

#include <cmath>


using namespace std;

namespace aserver {
namespace generator {


Primitive::Primitive(Core *core, uint32_t periodSize) : Generator(core, periodSize)
{
    PrimitiveConfigData *cfgData = new PrimitiveConfigData();
    cfgData->flags = primitiveConfigFlags::PRIMITIVE_ALL; //configure all data fields in constructor
    config(cfgData);
}

void Primitive::config(const ConfigData *configData)
{
    PrimitiveConfigData* cfgData = (PrimitiveConfigData*) configData;

    if (cfgData->flags & primitiveConfigFlags::PRIMITIVE_AMPLITUDE) {
        amplitude = cfgData->amplitude;
    }
    if (cfgData->flags & primitiveConfigFlags::FREQUENCY) {
        frequency = cfgData->frequency;
    }
    if (cfgData->flags & primitiveConfigFlags::SQUARE_FACTOR) {
        squareFactor = cfgData->squareFactor;
    }
    if (cfgData->flags & primitiveConfigFlags::WAVEFORM) {
        waveform = cfgData->waveform;
    }
}

void Primitive::renderNFrames(uint32_t start, uint32_t end)
{
    uint32_t fs = 44100;
    double T = 1./fs;
    int16_t sams[2];

    switch (waveform) {
        case waveformType::SINE:
            for (uint32_t i = start; i < end; i++) {
                phase += 2. * M_PI * frequency * T;
                sams[0] = sams[1] = (int16_t) (amplitude * cos(phase));
                buffer->writeFrame(sams, i);
            }
            break;
        case waveformType::SQUARE:
            for (uint32_t i = start; i < end; i++) {
                phase += 2. * M_PI * frequency * T;
                sams[0] = sams[1] = (int16_t) (amplitude * tanh(sin(phase) * squareFactor));
                buffer->writeFrame(sams, i);
            }
            break;
        case waveformType::SAWTOOTH:
            for (uint32_t i = start; i < end; i++) {
                phase = fmod(phase + (2. * frequency * T), 2.);
                sams[0] = sams[1] = (int16_t) (amplitude * (phase - 1.));
                buffer->writeFrame(sams, i);
            }
            break;
    }
}

void Primitive::render()
{
    renderNFrames(0, buffer->getPeriodSize());
}

} //end generator namespace
} //end aserver namespace
