#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include "output.h"
#include "../sound_buffer.h"

namespace aserver {
namespace output {

class Memory :public Output {
    private:
        vector<int16_t> outputData;
    public:
        Memory(Core *core, uint32_t periodSize) : Output(core, periodSize) {};
        void write(SoundBuffer *buffer) override;
        void close() {};
        uint64_t get_output(int16_t **dest);
};

} //end namespace output
} //end namespace aserver

#endif
