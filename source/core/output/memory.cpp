#include "memory.h"
#include "../wav_file.h"
#include <fstream>
#include <bits/stream_iterator.h>
#include <iostream>

using namespace std;

namespace aserver {
namespace output {

void Memory::write(SoundBuffer &buffer)
{
    outputData.insert(outputData.end(), buffer.getData(), buffer.getData() + (buffer.getPeriodSize() * buffer.getFrameSize()));
}

void Memory::close()
{
    outputData.clear();
}

} //end namespace output
} //end namespace aserver