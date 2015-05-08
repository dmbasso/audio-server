#ifndef ALSA_H
#define ALSA_H

#include "output.h"
#include "../sound_buffer.h"
#include <alsa/asoundlib.h>

namespace aserver {
namespace output {

class Alsa :public Output {
    private:
        snd_pcm_t *alsa_handle = nullptr;

    public:
        Alsa();
        void setup(int rate, int channels, snd_pcm_uframes_t frames);
        operator bool () {return alsa_handle != nullptr;}
        void write(SoundBuffer &buffer) override;
        void close() override;
        int  avail();
        int  delay();
};

} //end namespace output
} //end namespace aserver

#endif