#ifndef FILE_H
#define FILE_H

#include <fstream>
#include "output.h"
#include "../sound_buffer.h"

namespace aserver {
namespace output {

class File :public Output {
    public:
        unsigned currentSize;
        fstream fs;

        File();
        void write(SoundBuffer &buffer) override;
        void close() override;
};

} //end namespace output
} //end namespace aserver

#endif