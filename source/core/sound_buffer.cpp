#include "sound_buffer.h"

#include <iostream>

using namespace std;

namespace aserver {

SoundBuffer::SoundBuffer(uint32_t _periodSize, uint16_t _frameSize)
{
    frameSize = _frameSize;
    periodSize = _periodSize;
    data = new int16_t[_periodSize * _frameSize];

    reset();
}

void SoundBuffer::reset()
{
    for (uint32_t i = 0; i < frameSize * periodSize; i++) {
        data[i] = 0;
    }
}

int16_t* SoundBuffer::readFrame(int16_t *sams, float position)
{
    int32_t p = (int32_t) position;
    float f = position - p;

    if (frameSize == 1) {
        sams[0] = sams[1] = (int16_t)((1 - f) * data[p] + f * data[p + 1]);
    }
    if (frameSize == 2) {
        sams[0] = (int16_t) ((1 - f) * data[p * 2] + f * data[p * 2 + 2]);
        sams[1] = (int16_t) ((1 - f) * data[p * 2 + 1] + f * data[p * 2 + 3]);
    }
    return sams;
}

void SoundBuffer::writeFrame(int16_t *sams, uint32_t i)
{
    if (i >= periodSize) {
        cout << "index out of range" << endl;
        return;
    }
    if (frameSize == 1) {
        data[i] = sams[0];
    }
    if (frameSize == 2) {
        data[i * 2] = sams[0];
        data[i * 2 + 1] = sams[1];
    }
}

void SoundBuffer::mixFrame(int16_t *sams, uint32_t i)
{
    if (i >= periodSize) {
        cout << "index out of range" << endl;
        return;
    }
    if (frameSize == 1) {
        data[i] += sams[0];
    }
    if (frameSize == 2) {
        data[i * 2] += sams[0];
        data[i * 2 + 1] += sams[1];
    }
}

} //end namespace aserver
