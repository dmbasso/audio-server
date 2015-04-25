#include "sound_buffer.h"

#include <iostream>

using namespace std;

namespace aserver {

SoundBuffer::SoundBuffer(unsigned _periodSize, unsigned short _frameSize)
{
    frameSize = _frameSize;
    periodSize = _periodSize;
    data = new int16_t[_periodSize * _frameSize];

    reset();
}

void SoundBuffer::reset()
{
    for (unsigned i = 0; i < frameSize * periodSize; i++) {
        data[i] = 0;
    }
}

int16_t* SoundBuffer::readFrame(int16_t *sams, unsigned i)
{
    if (frameSize == 1) {
        sams[0] = data[i];
    }
    if (frameSize == 2) {
        sams[0] = data[i * 2];
        sams[1] = data[i * 2 + 1];
    }
    return sams;
}

void SoundBuffer::writeFrame(int16_t *sams, unsigned i)
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

void SoundBuffer::mixFrame(int16_t *sams, unsigned i)
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
