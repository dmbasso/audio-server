#ifndef WAV_FILE_H
#define WAV_FILE_H

#include <fstream>

using namespace std;

void writeWavHeader(ofstream *ofs, unsigned int currentSize);

#endif