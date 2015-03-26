#ifndef OUTPUT_H
#define OUTPUT_H

#include <fstream>

#include "generator.h"

using namespace std;

typedef struct {
    char     chunk_id[4];
    uint32_t chunk_size;
    char     format[4];
    char     fmtchunk_id[4];
    uint32_t fmtchunk_size;
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bps;
    char     datachunk_id[4];
    uint32_t datachunk_size;
}WavHeader;


namespace aserver {

/** \brief Contains classes for saving or reproducing the processed audio.
 *
 */
namespace output {

class Output {
    public:
        virtual void write(SoundBuffer &buffer) =0;
        virtual void close() =0;
};

class File :public Output {
    public:
        unsigned current_size;
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
