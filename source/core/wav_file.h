#ifndef WAV_FILE_H
#define WAV_FILE_H

#include <fstream>
#include "sound_buffer.h"

using namespace std;

namespace aserver {

void writeWavHeader(fstream *fs, unsigned int currentSize);
SoundBuffer* loadWave(const string filename);
void normalise(fstream *fs, unsigned int currentSize);

} // end namespace aserver

#endif