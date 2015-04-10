#include "sound_buffer.h"

#include <iostream>
#include <cmath>

using namespace std;

namespace aserver {

SoundBuffer::SoundBuffer(unsigned _periodSize)
{
    this->frameSize = 2;
    this->periodSize = _periodSize;
    this->data = new int16_t[_periodSize * frameSize];
    this->bufHead = 0;

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

void SoundBuffer::pushFrame(int16_t *sams)
{
    if (frameSize == 1) {
        data[bufHead] = sams[0];
    }
    if (frameSize == 2) {
        data[bufHead * 2] = sams[0];
        data[bufHead * 2 + 1] = sams[1];
    }

    bufHead = ++bufHead % periodSize;
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
