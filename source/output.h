#ifndef OUTPUT_H
#define OUTPUT_H

#include <fstream>

#include "generator.h"
#include "wav_file.h"

using namespace std;

namespace aserver {

/** \brief Contains classes for saving or reproducing the processed audio.
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

class File :public Output {
    public:
        unsigned currentSize;
        ofstream ofs;

    public:
        File();
        void write(SoundBuffer &buffer) override;
        void close() override;
};

class Alsa :public Output {
    public:
        void write(SoundBuffer &buffer) override;
        void close() {};
};

class Memory :public Output {
    public:
        void write(SoundBuffer &buffer) override;
        void close() {};
};

} //end namespace output
} //end namespace aserver

#endif
