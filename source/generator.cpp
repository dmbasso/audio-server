#include "generator.h"

#include <cmath>

using namespace std;

namespace aserver {

SoundBuffer::SoundBuffer(unsigned period_size)
{
    this->frame_size = 2;
    this->period_size = period_size;
    this->data = new int16_t[period_size * frame_size];

    for (int i = 0; i < frame_size * period_size; i++)
        data[i] = 0;
}

void SoundBuffer::reset()
{
    for (int i = 0; i < frame_size*period_size; i++)
        data[i] = 0;
}

namespace generator {

Generator::Generator(unsigned period_size)
{
    this->buffer = new SoundBuffer(period_size);
    this->location = Location(0., 10., 0.);
}

Primitive::Primitive(unsigned period_size) : Generator(period_size)
{
    this->phase = 0;
    this->frequency = 440;
    this->amplitude = 32767;
    this->squareFactor = 2;
    this->time_index = 0;
}

void Primitive::config(const ConfigData *configdata)
{
    PrimitiveConfigData* pcd = (PrimitiveConfigData*) configdata;

    this->amplitude = pcd->amplitude;
    this->phase = pcd->phase;
    this->frequency = pcd->frequency;
    this->squareFactor = pcd->squareFactor;

    this->location = pcd->location; //use copy contructor for Location?
}

void Primitive::render()
{
    // renders a wave as: sample = (int16) (tanh(sin(phase) * squareFactor) * amplitude);
    // squareFactor >= 1, amplitude < 32767 (if sample is int16)

    //for now lets render a real sin waveform
    unsigned fs = 44100;
    float T = 1./fs;

    for (int i = 0; i < this->buffer->getPeriodSize(); i++) {
        int16_t sam = amplitude * cos(2. * M_PI * frequency * T * (time_index + i) + phase);
        this->buffer->data[i*2] = sam;
        this->buffer->data[i*2 + 1] = sam;
        //cout << "data[" << (i*2)+time_index << "] = " << this->buffer->data[i*2] << " - data[" << (i*2+1) + time_index << "] = " << this->buffer->data[i*2+1] << endl;
    }
    time_index += this->buffer->getPeriodSize();
}

void Test::render()
{

}

Wave::Wave(unsigned period_size) : Generator(period_size)
{

}

Wave::~Wave()
{

}

void Wave::config(const ConfigData *configdata)
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
