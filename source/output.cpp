#include <string.h>

#include "output.h"

using namespace std;

namespace aserver {
namespace output {

File::File()
{
    ofs.open("output/output.wav", std::ofstream::out);
    writeHeader();
}

void File::write(SoundBuffer &buffer)
{
    ofs.write(reinterpret_cast<char *>(buffer.getData()), 2 * 2 * buffer.getPeriodSize());
    currentSize += 2 * 2 * buffer.getPeriodSize();
}

void File::writeHeader()
{
    strncpy(header.chunkId,"RIFF",4);
    header.datachunkSize = currentSize;
    header.chunkSize = header.datachunkSize + 36;
    strncpy(header.format,"WAVE",4);
    header.numChannels = 2;
    strncpy(header.fmtchunkId,"fmt ",4);
    header.fmtchunkSize = 16;
    header.audioFormat = 1;
    header.sampleRate = 44100;
    header.byteRate = (header.sampleRate * header.bps * header.numChannels) / 8;
    header.blockAlign = header.numChannels * (header.bps >> 3);
    header.bps = 16;
    strncpy(header.datachunkId,"data",4);

    ofs.seekp(0);
    ofs.write((char *) &header, sizeof(WavHeader));
}

void File::close()
{
    writeHeader();
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
