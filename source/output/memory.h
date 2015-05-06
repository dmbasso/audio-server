#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include "output.h"
#include "../sound_buffer.h"

namespace aserver {
namespace output {

class Memory :public Output {
    public:
        vector<int16_t> outputData;

        void write(SoundBuffer &buffer) override;
        void close() override;
};

} //end namespace output
} //end namespace aserver

#endif