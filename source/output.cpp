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
    ofs.write(reinterpret_cast<char *>(buffer.data), 2 * 2 * buffer.getPeriodSize());
    current_size += 2 * 2 * buffer.getPeriodSize();
}

void File::writeHeader()
{
    strncpy(header.chunk_id,"RIFF",4);
    header.datachunk_size = current_size;
    header.chunk_size = header.datachunk_size + 36;
    strncpy(header.format,"WAVE",4);
    header.num_channels = 2;
    strncpy(header.fmtchunk_id,"fmt ",4);
    header.fmtchunk_size = 16;
    header.audio_format = 1;
    header.sample_rate = 44100;
    header.byte_rate = (header.sample_rate * header.bps * header.num_channels) / 8;
    header.block_align = header.num_channels * (header.bps >> 3);
    header.bps = 16;
    strncpy(header.datachunk_id,"data",4);

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
