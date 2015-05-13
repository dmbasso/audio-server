#ifndef OUTPUT_BASE_H
#define OUTPUT_BASE_H

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

struct ConfigData {
    uint64_t flags;
};

class Output {
    public:
        virtual void config(ConfigData *outputData) {};
        virtual void write(SoundBuffer &buffer) {};
        virtual void close() {};
};

} //end namespace output
} //end namespace aserver

#endif

