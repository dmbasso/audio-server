#ifndef FILE_H
#define FILE_H

#include <fstream>
#include "output.h"
#include "../sound_buffer.h"

namespace aserver {
namespace output {

struct FileOutputConfigData : ConfigData {
    string outputFilePath;
};

class File :public Output {
    public:
        unsigned currentSize;
        fstream fs;
        string outputFilePath;

        File();
        void config(ConfigData *cfgData);
        void write(SoundBuffer &buffer) override;
        void close() override;
};

} //end namespace output
} //end namespace aserver

#endif
