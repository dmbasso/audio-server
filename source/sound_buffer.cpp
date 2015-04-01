#include "sound_buffer.h"

using namespace std;

namespace aserver {

SoundBuffer::SoundBuffer(unsigned _periodSize)
{
    this->frameSize = 2;
    this->periodSize = _periodSize;
    this->data = new int16_t[_periodSize * frameSize];

    reset();
}

void SoundBuffer::reset()
{
    for (int i = 0; i < frameSize * periodSize; i++) {
        data[i] = 0;
    }
}

void write(const int16_t *buffer)
{

}

int16_t* read()
{

}

} //end namespace aserver
