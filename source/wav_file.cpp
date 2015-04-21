#include "string.h"
#include <iostream>
#include <algorithm>
#include <valarray>

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
} wavHeader;

namespace aserver {

void writeWavHeader(ofstream *ofs, unsigned int currentSize) {
    wavHeader header;

    strncpy(header.chunkId, "RIFF", 4);
    header.datachunkSize = currentSize;
    header.chunkSize = header.datachunkSize + 36;
    strncpy(header.format, "WAVE", 4);
    header.numChannels = 2;
    strncpy(header.fmtchunkId, "fmt ", 4);
    header.fmtchunkSize = 16;
    header.audioFormat = 1;
    header.sampleRate = 44100;
    header.byteRate = (header.sampleRate * header.bps * header.numChannels) / 8;
    header.blockAlign = header.numChannels * (header.bps >> 3);
    header.bps = 16;
    strncpy(header.datachunkId, "data", 4);

    ofs->seekp(0);
    ofs->write((char *) &header, sizeof(wavHeader));
}

    /** \brief Reads a wavfile names /c filename and returns a pointer to a SoundBuffer that contains
     * sound data present in the wavfile.
     */

SoundBuffer* loadWave(const string filename) {
    ifstream ifs;
    string filePath = "input/" + filename;

    ifs.open(filePath, ifstream::in);
    if (ifs.is_open()) {
        wavHeader wh;
        ifs.read(reinterpret_cast<char *>(&wh), sizeof(wavHeader));
        cout << "\nReading wavfile = " << filename << "\nDatachunkSize = " << wh.datachunkSize;
        cout << "\nnumChannels = " << wh.numChannels << "\nSoundbuffer period size = " <<
        wh.datachunkSize / (wh.numChannels * 2) << endl << endl;

        SoundBuffer *sb = new SoundBuffer(wh.datachunkSize / (wh.numChannels * 2), wh.numChannels);
        ifs.read(reinterpret_cast<char *>(sb->getData()), wh.datachunkSize);
        return sb;
    }
    else {
        cout << "\nError opening file " << filename << endl;
        return 0;
    }
}

void normalise(fstream *fs, unsigned int currentSize)
{
    fs->seekp(sizeof(wavHeader));
    cout << "file ptr = " << fs->tellp() << endl;

    valarray<int16_t> temp = valarray<int16_t>(currentSize);
    for (unsigned i = 0; i < currentSize; i++) {
        fs->read(reinterpret_cast<char *>(&temp[i]), sizeof(int16_t));
    }

    int16_t max = *max_element(begin(temp), end(temp));
    cout << "temp[0] = " << temp[90] << endl;

    double normFactor = 32767./max;
    temp *= normFactor;

    cout << "temp[0] = " << temp[90] << endl;

    fs->seekp(sizeof(wavHeader));
    for (unsigned i = 0; i < currentSize; i++) {
        fs->write(reinterpret_cast<char *>(&temp[i]), sizeof(int16_t));
    }
}

}// end namespace aserver