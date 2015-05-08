#include "primitive.h"

#include <cmath>

using namespace std;

namespace aserver {
namespace generator {


Primitive::Primitive(unsigned periodSize) : Generator(periodSize)
{
    phase = 1.;
    frequency = 220.;
    amplitude = 3276;
    squareFactor = 2;
    waveform = generator::waveformType::SINE;
}

void Primitive::config(const ConfigData *configData)
{
    PrimitiveConfigData* pcd = (PrimitiveConfigData*) configData;
    amplitude = pcd->amplitude;
    frequency = pcd->frequency;
    squareFactor = pcd->squareFactor;
    waveform = pcd->waveform;
}

void Primitive::renderNFrames(unsigned start, unsigned end)
{
    unsigned fs = 44100;
    float T = 1./fs;
    int16_t sams[2];

    switch (waveform) {
        case waveformType::SINE:
            for (unsigned i = start; i < end; i++) {
                phase += 2. * M_PI * frequency * T;
                sams[0] = sams[1] = (int16_t) amplitude * cos(phase);
                buffer->writeFrame(sams, i);
            }
            break;
        case waveformType::SQUARE:
            for (unsigned i = start; i < end; i++) {
                phase += M_PI * frequency * T;
                sams[0] = sams[1] = (int16_t) amplitude * tanh(sin(phase) * squareFactor);
                buffer->writeFrame(sams, i);
            }
            break;
        case waveformType::SAWTOOTH:
            for (unsigned i = start; i < end; i++) {
                phase = fmod(phase + (2. * frequency * T), 2.);
                sams[0] = sams[1] = (int16_t) amplitude * (phase - 1.);
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
