#ifndef SOUND_BUFFER_H
#define SOUND_BUFFER_H

#include <cstdint>
#include <vector>

using namespace std;

namespace aserver {

/** \brief SoundBuffer is a container for blocks of size \c periodSize of audio frames with size \c frameSize.
 *
 *  \c periodSize corresponds to the size of each period of audio frames that are processed.
 *  It is set by the Core class in the server initialization.
 *  \c frameSize is the number of 16 bit audio samples per audio frame (mono = 1, stereo = 2 (default));
 *  \c data is a pointer to an array of \c int16_t type variables. It contains the 16 bit audio samples correnponding to one period. The SoundBuffer constructor initialization allocates this pointer, an \c int16_t vector of size = \c frameSize * \c periodSize, and zeros all values;
 *
 */

class SoundBuffer {
    private:
        unsigned short frameSize;
        unsigned periodSize;
        int16_t *data;

    public:
        unsigned bufHead;
        SoundBuffer(unsigned _periodSize);

        unsigned getPeriodSize() {return periodSize;}
        int16_t *getData() {return this->data;}
        void reset();
        void writeFrame(int16_t *sams, unsigned i);
        void pushFrame(int16_t *sams);
        int16_t* readFrame(int16_t *sams, unsigned i); //\todo interpolate (linear) read ->get_stereo_frame
        void mixFrame(int16_t *sams, unsigned i);
};

} //end namespace aserver

#endif