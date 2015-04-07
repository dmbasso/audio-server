#include "generator.h"

#include <cmath>

using namespace std;

namespace aserver {
namespace generator {

Generator::Generator(unsigned periodSize)
{
    this->buffer = new SoundBuffer(periodSize);
    this->location = Location(0., 10., 0.);
}

Primitive::Primitive(unsigned periodSize) : Generator(periodSize)
{
    this->phase = 0;
    this->frequency = 440;
    this->amplitude = 3277;
    this->squareFactor = 2;
    this->timeIndex = 0;
    this->waveform = generator::waveformType::SIN;
}

void Primitive::config(const ConfigData *configData)
{
    PrimitiveConfigData* pcd = (PrimitiveConfigData*) configData;

    this->amplitude = pcd->amplitude;
    this->phase = pcd->phase;
    this->frequency = pcd->frequency;
    this->squareFactor = pcd->squareFactor;

    this->location = pcd->location;
    this->waveform = pcd->wft;
}

    //\todo remove timeIndex and update signal with phase increment
    //\todo remove sin phase

void Primitive::render()
{
    unsigned fs = 44100;
    float T = 1./fs;
    float omega = M_PI * frequency * T;
    float sawPhaseIncrement = (frequency * 2.) / fs;

    int16_t sams[2] = {0, 0};

    switch (this->waveform) {
        case waveformType::SIN:
            for (int i = 0; i < this->buffer->getPeriodSize(); i++) {
                sams[0] = sams[1] = (int16_t) amplitude * cos(2. * omega * (timeIndex + i) + phase);
                this->buffer->writeFrame(sams, i);
            }
            break;
        case waveformType::SQUARE:
            for (int i = 0; i < this->buffer->getPeriodSize(); i++) {
                sams[0] = sams[1] = (int16_t) amplitude * tanh(sin((timeIndex + i) * omega) * squareFactor);
                this->buffer->writeFrame(sams, i);
            }
            break;
        case waveformType::SAWTOOTH:
            for (int i = 0; i < this->buffer->getPeriodSize(); i++) {
                lastSampleVal = fmod(lastSampleVal + sawPhaseIncrement, 2.);
                sams[0] = sams[1] = (int16_t) amplitude * (lastSampleVal - 1.);
                this->buffer->writeFrame(sams, i);
                //cout << "data[" << (i*2)+timeIndex << "] = " << sams[0] << " - data[" << (i*2+1) + timeIndex << "] = " << sams[1] << endl;
            }
            break;
    }
    timeIndex += this->buffer->getPeriodSize();
}

    //\todo test.render must check possible frequency changes within the buffer and execute them
    //      this should not copy primitive.render, instead it should use it and process freq changes

void Test::render()
{

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
