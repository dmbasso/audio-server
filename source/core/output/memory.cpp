#include "memory.h"
#include "../wav_file.h"
#include <fstream>
#include <bits/stream_iterator.h>
#include <iostream>
#include <algorithm>

using namespace std;

namespace aserver {
namespace output {

void Memory::write(SoundBuffer &buffer)
{
    outputData.insert(outputData.end(), buffer.getData(), buffer.getData() + (buffer.getPeriodSize() * buffer.getFrameSize()));
}

void Memory::close()
{
    //outputData.clear(); // <----- this should be elsewhere (like in a shutdown method)
}

uint64_t Memory::get_output(int16_t **dest)
{
    *dest = new int16_t[outputData.size()];
    copy(outputData.begin(), outputData.end(), *dest);
    return outputData.size();
}

} //end namespace output
} //end namespace aserver
