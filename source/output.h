#ifndef OUTPUT_H
#define OUTPUT_H

#include <fstream>

#include "generator.h"

using namespace std;

typedef struct {
    char     chunkId[4];
    uint32_t chunkSize;
    char     format[4];
    char     fmtchunkId[4];
    uint32_t fmtchunkSize;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bps;
    char     datachunkId[4];
    uint32_t datachunkSize;
}WavHeader;


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
        WavHeader header;
        ofstream ofs;

    public:
        File();
        void write(SoundBuffer &buffer) override;
        void close() override;
        void writeHeader();
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
