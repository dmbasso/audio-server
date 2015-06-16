#include "memory.h"
#include "../wav_file.h"
#include <fstream>
#include <bits/stream_iterator.h>
#include <iostream>
#include <algorithm>

using namespace std;

namespace aserver {
namespace output {

void Memory::write(SoundBuffer *buffer)
{
    if (buffer) {
        outputData.insert(outputData.end(), buffer->getData(),
                          buffer->getData() + (buffer->getPeriodSize() * buffer->getFrameSize()));
    } else {
        outputData.insert(outputData.end(), silence->getData(),
                          silence->getData() + (silence->getPeriodSize() * silence->getFrameSize()));
    }
}

uint64_t Memory::get_output(int16_t **dest, bool clear)
{
    *dest = new int16_t[outputData.size()];
    copy(outputData.begin(), outputData.end(), *dest);
    uint64_t outputSize = outputData.size();
    if (clear) {
        outputData.clear();
    }
    return outputSize;
}

} //end namespace output
} //end namespace aserver
