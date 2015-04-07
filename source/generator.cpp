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

    this->amplitude = pcd->getAmplitude();
    this->phase = pcd->getPhase();
    this->frequency = pcd->getFrequency();
    this->squareFactor = pcd->getSquareFactor();

    this->location = pcd->getLocation(); //use copy constructor for Location?
    this->waveform = pcd->getWaveformType();

}

    //\todo move static calculation part out of the loop...
    //\todo take switch statement out of the for loop

void Primitive::render()
{
    int16_t *tempBuffer = this->buffer->read();
    unsigned fs = 44100;
    float T = 1./fs;
    float omega = M_PI * frequency * T;
    float sawPhaseIncrement = (frequency * 2.) / fs;
    int16_t sam = 0;

    for (int i = 0; i < this->buffer->getPeriodSize(); i++) {
        switch (this->waveform) {
            case waveformType::SIN:
                sam = (int16_t) amplitude * cos(2. * omega * (timeIndex + i) + phase);
                break;
            case waveformType::SQUARE:
                sam = (int16_t) amplitude * tanh( sin((timeIndex + i) * omega) * squareFactor);
                break;
            case waveformType::SAWTOOTH:
                lastSampleVal = fmod(lastSampleVal + sawPhaseIncrement, 2.);
                sam = (int16_t) amplitude * (lastSampleVal - 1.);
                break;
        }
        tempBuffer[i*2] = sam;
        tempBuffer[i*2 + 1] = sam;
        //cout << "data[" << (i*2)+timeIndex << "] = " << tempBuffer[i*2] << " - data[" << (i*2+1) + timeIndex << "] = " << tempBuffer[i*2+1] << endl;
    }
    timeIndex += this->buffer->getPeriodSize();
}

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
