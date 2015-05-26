#ifndef WAV_FILE_H
#define WAV_FILE_H

#include <fstream>
#include "sound_buffer.h"

using namespace std;

namespace aserver {

void writeWavHeader(fstream *fs, uint32_t currentSize);
SoundBuffer* loadWave(const char *filename);
void normalise(fstream *fs, uint32_t currentSize);

} // end namespace aserver

#endif