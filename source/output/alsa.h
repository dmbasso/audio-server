#ifndef ALSA_H
#define ALSA_H

#include "output.h"
#include "../sound_buffer.h"

namespace aserver {
namespace output {

class Alsa :public Output {
    public:
        void write(SoundBuffer &buffer) override;
        void close() override;
};

} //end namespace output
} //end namespace aserver

#endif