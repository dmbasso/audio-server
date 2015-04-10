#include "output.h"

using namespace std;

namespace aserver {
namespace output {

File::File()
{
    ofs.open("output/output.wav", std::ofstream::out);
    writeWavHeader(&ofs, 0);
}

void File::write(SoundBuffer &buffer)
{
    ofs.write(reinterpret_cast<char *>(buffer.getData()), 2 * 2 * buffer.getPeriodSize());
    currentSize += 2 * 2 * buffer.getPeriodSize();
}

    //\todo Normalize output file?

void File::close()
{
    writeWavHeader(&ofs, this->currentSize);
    ofs.close();
}

void Alsa::write(SoundBuffer &buffer)
{

}

void Memory::write(SoundBuffer &buffer)
{

}

} //end namespace output
} //end namespace aserver
