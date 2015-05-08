#ifndef OUTPUT_BASE_H
#define OUTPUT_BASE_H

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

struct OutputConfigData {

};

class Output {
    public:
        virtual void config(OutputConfigData *outputData) {};
        virtual void write(SoundBuffer &buffer) {};
        virtual void close() {};
};

} //end namespace output
} //end namespace aserver

#endif

