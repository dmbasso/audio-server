#ifndef OUTPUT_H
#define OUTPUT_H

#include <fstream>

#include "../sound_buffer.h"

using namespace std;

namespace aserver {

/** \brief Contains classes for storing or reproducing the processed audio.
 *
 */
namespace output {

enum class types : int {
    FILE = 1,
    ALSA = 2,
    MEMORY = 3
};

class Output {
    public:
        virtual void write(SoundBuffer &buffer) =0;
        virtual void close() =0;
};

} //end namespace output
} //end namespace aserver

#endif
