#include "generator.h"

#include <cmath>
#include <iostream>

using namespace std;

namespace aserver {
namespace generator {

    //\todo locations must become a map<unsigned bufferIndex, Location loc> that stores
    // location changes in each buffer
    // This map will be used to update the location of each source when rendered (source.render)
    // For that we must create a map iterator to check the next map pair for location changes

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
    this->loc = Location();
}

void Primitive::config(const ConfigData *configData)
{
    PrimitiveConfigData* pcd = (PrimitiveConfigData*) configData;
    this->amplitude = pcd->amplitude;
    this->frequency = pcd->frequency;
    this->squareFactor = pcd->squareFactor;
    this->waveform = pcd->wft;
    this->loc = pcd->location;
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
    remainingFrames = transitionPeriod;
    //frequencyScaleFactor = 1.05946; //half tone scaling factor
    frequencyScaleFactor = 1.5; //fifth scaling factor
    //frequencyScaleFactor = 2.; //octave scaling factor
    locationScaleFactor = 0.;
}

    //\todo test.render must check possible frequency changes within the buffer and execute them

void Test::render()
{
    locs.clear();

    unsigned startIndex = 0;

    for (unsigned i = 0; i < buffer->getPeriodSize(); i++) {
        if (remainingFrames == 0) {
            locs[i] = loc.sumY(locationScaleFactor);
            Primitive::renderNFrames(startIndex, i);
            startIndex = i;
            frequency *= frequencyScaleFactor;
            remainingFrames = transitionPeriod;
            cout << "frequency/location change: " << loc.toString() << " Frequency = " << frequency << endl;
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
