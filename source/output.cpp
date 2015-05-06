#include <iostream>
#include "output.h"

using namespace std;

namespace aserver {
namespace output {

File::File()
{
    fs.open("audio/output/output.wav", fstream::out | fstream::in | fstream::trunc);
    writeWavHeader(&fs, 0);
}

void File::write(SoundBuffer &buffer)
{
    fs.write(reinterpret_cast<char *>(buffer.getData()), 2 * 2 * buffer.getPeriodSize());
    currentSize += 2 * 2 * buffer.getPeriodSize();
}

void File::close()
{
    writeWavHeader(&fs, currentSize);
    normalise(&fs, currentSize);
    fs.close();
}

void Alsa::write(SoundBuffer &buffer)
{

}

void Memory::write(SoundBuffer &buffer)
{

}

} //end namespace output
} //end namespace aserver
