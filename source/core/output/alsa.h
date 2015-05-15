#ifndef ALSA_H
#define ALSA_H

#include "output.h"
#include "../sound_buffer.h"
#include <alsa/asoundlib.h>

namespace aserver {
namespace output {

enum alsaConfigFlags : uint64_t {
    WITH_PULSEAUDIO =   0x1,
    SAMPLING_RATE =     0x2,
    N_CHANNELS =        0x4,
    ALSA_ALL =          0X7
};

struct AlsaOutputConfigData : ConfigData {
    bool withPulseAudio = true;
    unsigned samplingRate = 44100;
    unsigned short nChannels = 2;
};

class Alsa :public Output {
    private:
        bool withPulseAudio;
        unsigned samplingRate;
        unsigned short nChannels;
        snd_pcm_t *alsa_handle = nullptr;

    public:
        Alsa();
        void setupWithPulseAudio(int rate, int channels);
        void setupNoPulseAudio(int rate, int channels, snd_pcm_uframes_t frames);
        operator bool () {return alsa_handle != nullptr;}
        void write(SoundBuffer &buffer) override;
        void config(ConfigData *cfgData);
        void close() override;
        int  avail();
        int  delay();
};

} //end namespace output
} //end namespace aserver

#endif