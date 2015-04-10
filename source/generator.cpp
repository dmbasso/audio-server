#include "generator.h"

#include <cmath>
#include <iostream>

using namespace std;

namespace aserver {
namespace generator {

Generator::Generator(unsigned periodSize)
{
    this->buffer = new SoundBuffer(periodSize);
    this->locationsInBuffer = vector<Location>(periodSize);
}

Primitive::Primitive(unsigned periodSize) : Generator(periodSize)
{
    this->phase = 1.;
    this->frequency = 220.;
    this->frequenciesInBuffer = vector<float>(periodSize, this->frequency);
    this->amplitude = 3276;
    this->squareFactor = 2;
    this->waveform = generator::waveformType::SINE;
    this->loc = Location();
    this->locationsInBuffer = vector<Location>(periodSize, Location());
}

void Primitive::config(const ConfigData *configData)
{
    PrimitiveConfigData* pcd = (PrimitiveConfigData*) configData;
    this->amplitude = pcd->amplitude;
    this->frequency = pcd->frequency;
    this->frequenciesInBuffer = vector<float>(buffer->getPeriodSize(), pcd->frequency);
    this->squareFactor = pcd->squareFactor;
    this->waveform = pcd->wft;
    this->loc = pcd->location;
    this->locationsInBuffer = vector<Location>(buffer->getPeriodSize(), pcd->location);
}

void Primitive::render()
{
    unsigned fs = 44100;
    float T = 1./fs;
    int16_t sams[2];

    switch (this->waveform) {
        case waveformType::SINE:
            for (unsigned i = 0; i < buffer->getPeriodSize(); i++) {
                phase += 2. * M_PI * frequenciesInBuffer[i] * T;
                sams[0] = sams[1] = (int16_t) amplitude * cos(phase);
                this->buffer->writeFrame(sams, i);
            }
            break;
        case waveformType::SQUARE:
            for (unsigned i = 0; i < buffer->getPeriodSize(); i++) {
                phase += M_PI * frequenciesInBuffer[i] * T;
                sams[0] = sams[1] = (int16_t) amplitude * tanh(sin(phase) * squareFactor);
                this->buffer->writeFrame(sams, i);
            }
            break;
        case waveformType::SAWTOOTH:
            for (unsigned i = 0; i < buffer->getPeriodSize(); i++) {
                phase = fmod(phase + (2. * frequenciesInBuffer[i] * T), 2.);
                sams[0] = sams[1] = (int16_t) amplitude * (phase - 1.);
                this->buffer->writeFrame(sams, i);
            }
            break;
    }
}

Test::Test(unsigned periodSize) : Primitive(periodSize)
{
    transitionPeriod = 22050;
    remainingFrames = transitionPeriod;
    //frequencyScaleFactor = 1.05946; //half tone scaling factor
    frequencyScaleFactor = 1.5; //fifth scaling factor
    //frequencyScaleFactor = 2.; //octave scaling factor
    locationScaleFactor = 5.;
}


    //\todo test.render must check possible frequency changes within the buffer and execute them
    //      this should not copy primitive.render, instead it should use it and process freq changes

void Test::render()
{
    for (unsigned i = 0; i < buffer->getPeriodSize(); i++) {
        if (remainingFrames == 0) {
            loc.sumY(locationScaleFactor);
            frequency *= frequencyScaleFactor;
            remainingFrames = transitionPeriod;
            cout << "frequency/location change: " << loc.toString() << " Frequency = " << frequency << endl;
        }
        frequenciesInBuffer[i] = frequency;
        locationsInBuffer[i] = loc;
        remainingFrames--;
    }
    Primitive::render();
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
