#ifndef ALSA_H
#define ALSA_H

#include "output.h"
#include "../sound_buffer.h"
#include <alsa/asoundlib.h>

namespace aserver {
namespace output {

enum alsaConfigFlags : uint64_t {
    RATE =          0x1,
    CHANNELS =      0x2,
    BLOCKING =      0x4,
    DUMP_CONFIG =   0x8
};

#pragma pack(1)  // force byte-alignment

struct AlsaOutputConfigData : ConfigData {
    uint32_t rate = 44100;
    uint16_t channels = 2;
};

#pragma pack()

class Alsa :public Output {
    private:
        uint32_t rate;
        uint16_t channels;
        bool blocking = true;
        snd_pcm_t *alsa_handle = nullptr;

    public:
        Alsa(Core *core, uint32_t periodSize);
        void setup();
        operator bool () {return alsa_handle != nullptr;}
        void write(SoundBuffer *buffer) override;
        void config(ConfigData *cfgData);
        void close() override;
        int32_t avail();
        int32_t delay();
};

} //end namespace output
} //end namespace aserver

#endif
