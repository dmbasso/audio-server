#ifndef FILE_H
#define FILE_H

#include <fstream>
#include "output.h"
#include "../sound_buffer.h"

namespace aserver {
namespace output {

enum fileConfigFlags : uint64_t {
    OUTPUT_FILEPATH =   0x1,
    NORMALISE =         0x2,
    FILE_ALL =          0X3
};

#pragma pack(1)  // force byte-alignment

struct FileOutputConfigData : ConfigData {
    char outputFilePath[256] = "output.wav"; // default output file path
    bool normalise_audio = false;
};

#pragma pack()

class File :public Output {
    public:
        unsigned currentSize;
        fstream fs;
        string outputFilePath;
        bool normalise_audio;

        File();
        void config(ConfigData *cfgData) override;
        void write(SoundBuffer &buffer) override;
        void close() override;
};

} //end namespace output
} //end namespace aserver

#endif
