#include "generator_primitive.h"

#include <cmath>

using namespace std;

namespace aserver {
namespace generator {


Primitive::Primitive(unsigned periodSize) : Generator(periodSize)
{
    this->phase = 1.;
    this->frequency = 220.;
    this->amplitude = 3276;
    this->squareFactor = 2;
    this->waveform = generator::waveformType::SINE;
}

void Primitive::config(const ConfigData *configData)
{
    PrimitiveConfigData* pcd = (PrimitiveConfigData*) configData;
    this->amplitude = pcd->amplitude;
    this->frequency = pcd->frequency;
    this->squareFactor = pcd->squareFactor;
    this->waveform = pcd->wft;
}

void Primitive::renderNFrames(unsigned start, unsigned end)
{
    unsigned fs = 44100;
    float T = 1./fs;
    int16_t sams[2];

    switch (this->waveform) {
        case waveformType::SINE:
            for (unsigned i = start; i < end; i++) {
                phase += 2. * M_PI * frequency * T;
                sams[0] = sams[1] = (int16_t) amplitude * cos(phase);
                this->buffer->writeFrame(sams, i);
            }
            break;
        case waveformType::SQUARE:
            for (unsigned i = start; i < end; i++) {
                phase += M_PI * frequency * T;
                sams[0] = sams[1] = (int16_t) amplitude * tanh(sin(phase) * squareFactor);
                this->buffer->writeFrame(sams, i);
            }
            break;
        case waveformType::SAWTOOTH:
            for (unsigned i = start; i < end; i++) {
                phase = fmod(phase + (2. * frequency * T), 2.);
                sams[0] = sams[1] = (int16_t) amplitude * (phase - 1.);
                this->buffer->writeFrame(sams, i);
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
