#ifndef OUTPUT_BASE_H
#define OUTPUT_BASE_H

#include "../sound_buffer.h"

using namespace std;

namespace aserver {

/** \brief Contains classes for storing or reproducing the processed audio.
 *
 */
namespace output {

enum class types : int32_t {
    FILE = 1,
    ALSA = 2,
    MEMORY = 3
};

#pragma pack(1)  // force byte-alignment

struct ConfigData {
    uint64_t flags;
};

#pragma pack()

class Output {
    public:
        virtual void config(ConfigData *outputData) {};
        virtual void write(SoundBuffer &buffer) {};
        virtual void close() {};
};

} //end namespace output
} //end namespace aserver

#endif

