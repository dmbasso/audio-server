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
    uint32_t samplingRate = 44100;
    uint16_t nChannels = 2;
};

class Alsa :public Output {
    private:
        bool withPulseAudio;
        uint32_t samplingRate;
        uint16_t nChannels;
        snd_pcm_t *alsa_handle = nullptr;

    public:
        Alsa(Core *core, uint32_t periodSize);
        void setupWithPulseAudio(int rate, int32_t channels);
        void setupNoPulseAudio(int rate, int32_t channels, snd_pcm_uframes_t frames);
        operator bool () {return alsa_handle != nullptr;}
        void write(SoundBuffer *buffer) override;
        void config(ConfigData *cfgData);
        void close() override;
        int32_t  avail();
        int32_t  delay();
};

} //end namespace output
} //end namespace aserver

#endif