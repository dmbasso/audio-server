#include "string.h"

#include "wav_file.h"
#include "wav_header.h"

using namespace std;

void writeWavHeader(ofstream *ofs, unsigned int currentSize)
{
    wavHeader header;

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

    ofs->seekp(0);
    ofs->write((char *) &header, sizeof(wavHeader));
}