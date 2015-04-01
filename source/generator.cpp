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
}

void Primitive::config(const ConfigData *configData)
{
    PrimitiveConfigData* pcd = (PrimitiveConfigData*) configData;

    this->amplitude = pcd->getAmplitude();
    this->phase = pcd->getPhase();
    this->frequency = pcd->getFrequency();
    this->squareFactor = pcd->getSquareFactor();

    this->location = pcd->getLocation(); //use copy constructor for Location?
}

    //\todo move static calculation part out of the loop...

void Primitive::render()
{
    // renders a wave as: sample = (int16) (tanh(sin(phase) * squareFactor) * amplitude);
    // squareFactor >= 1, amplitude < 32767 (if sample is int16)

    int16_t *tempBuffer = this->buffer->read();
    unsigned fs = 44100;
    float T = 1./fs;
    float sawPhaseInc = (frequency * 2.) / fs;
    int16_t sam = 0;

    for (int i = 0; i < this->buffer->getPeriodSize(); i++) {
        // sin waveform
        sam = (int16_t) amplitude * cos(2. * M_PI * frequency * T * (timeIndex + i) + phase);
        // square waveform
        //sam = (int16_t) amplitude * tanh( sin((timeIndex + i) * M_PI * frequency * T) * squareFactor);
        //cout << "data[" << (i*2)+timeIndex << "] = " << tempBuffer[i*2] << " - data[" << (i*2+1) + timeIndex << "] = " << tempBuffer[i*2+1] << endl;
        // sawtooth waveform
        //lastSawVal = fmod(lastSawVal + sawPhaseInc, 2.);
        //sam = (int16_t) amplitude * (lastSawVal - 1.);

        tempBuffer[i*2] = sam;
        tempBuffer[i*2 + 1] = sam;
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
