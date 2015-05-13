#ifndef FILE_H
#define FILE_H

#include <fstream>
#include "output.h"
#include "../sound_buffer.h"

namespace aserver {
namespace output {

enum fileConfigFlags : uint64_t {
    OUTPUT_FILEPATH =   0x1,
    FILE_ALL =          0X1
};

struct FileOutputConfigData : ConfigData {
    const char *outputFilePath = "output.wav"; // default output file path
};

class File :public Output {
    public:
        unsigned currentSize;
        fstream fs;
        const char *outputFilePath;

        File();
        void config(ConfigData *cfgData);
        void write(SoundBuffer &buffer) override;
        void close() override;
};

} //end namespace output
} //end namespace aserver

#endif
