#include "string.h"
#include <fstream>

#include "wav_file.h"

using namespace std;

typedef struct {
    char     chunkId[4];
    uint32_t chunkSize;
    char     format[4];
    char     fmtchunkId[4];
    uint32_t fmtchunkSize;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bps;
    char     datachunkId[4];
    uint32_t datachunkSize;
} WavHeader;

void writeWavHeader(ofstream *ofs, unsigned int currentSize)
{
    WavHeader header;

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
    ofs->write((char *) &header, sizeof(WavHeader));
}