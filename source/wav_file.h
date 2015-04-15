#ifndef WAV_HEADER_H
#define WAV_HEADER_H

#include <fstream>

using namespace std;

void writeWavHeader(ofstream *ofs, unsigned int currentSize);

#endif