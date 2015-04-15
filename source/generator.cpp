#include "generator.h"

#include <cmath>
#include <iostream>

using namespace std;

namespace aserver {
namespace generator {

Generator::Generator(unsigned periodSize)
{
    this->buffer = new SoundBuffer(periodSize);
}

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

Test::Test(unsigned periodSize) : Primitive(periodSize)
{
    transitionPeriod = 22050;
    remainingFrames = 0;
    //frequencyScaleFactor = 1.05946; //half tone scaling factor
    frequencyScaleFactor = 1.5; //fifth scaling factor

    distance = 5.;
    angleStep = M_PI/4.;
    initialAngle = M_PI/2.;
    currentAngle = initialAngle;
}

/** \brief Generates a clockwise rotation for a generator.
 * Default position is along the Y axis (front).
 */

void Test::render()
{
    unsigned startIndex = 0;

    locs.clear();
    buffer->reset();

    for (unsigned i = 0; i < buffer->getPeriodSize(); i++) {
        if (remainingFrames == 0) {
            if ((-2. * M_PI) + initialAngle  - currentAngle == 0) {
                Primitive::renderNFrames(startIndex, i);
                return;
            }

            locs[i] = Location::computeTestPosition(distance, currentAngle);
            Primitive::renderNFrames(startIndex, i);

            cout << "Frequency = " << frequency << " -> " << locs[i].toString() << endl;

            startIndex = i;
            this->currentAngle -= this->angleStep;
            frequency *= frequencyScaleFactor;
            remainingFrames = transitionPeriod;
        }
        remainingFrames--;
    }

    if (startIndex != buffer->getPeriodSize()) {
        Primitive::renderNFrames(startIndex, buffer->getPeriodSize());
    }
}

void Test::config(const ConfigData *configData)
{
    Primitive::config(configData);
}

Wave::Wave(unsigned periodSize) : Generator(periodSize)
{

}

Wave::~Wave()
{

}

void Wave::config(const ConfigData *configData)
{

}

void Wave::render()
{

}

void Script::render ()
{

}

} //end generator namespace
} //end aserver namespace
