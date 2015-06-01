#include <iostream>
#include "alsa.h"

using namespace std;

namespace aserver {
namespace output {

Alsa::Alsa(Core *core, uint32_t periodSize) : Output(core, periodSize)
{
    AlsaOutputConfigData *cfgData = new AlsaOutputConfigData();
    cfgData->flags = alsaConfigFlags::ALSA_ALL;
    config(cfgData);
}

void Alsa::config(ConfigData *configData)
{
    AlsaOutputConfigData *cfgData = (AlsaOutputConfigData *) configData;

    if(cfgData->flags & alsaConfigFlags::SAMPLING_RATE) {
        samplingRate = cfgData->samplingRate;
    }
    if(cfgData->flags & alsaConfigFlags::N_CHANNELS) {
        nChannels = cfgData->nChannels;
    }
    if(cfgData->flags & alsaConfigFlags::WITH_PULSEAUDIO) {
        withPulseAudio = cfgData->withPulseAudio;
        if (withPulseAudio) {
            setupWithPulseAudio(samplingRate, nChannels);
        }
        else {
            setupNoPulseAudio(samplingRate, nChannels, 8192);
        }
    }
}

void Alsa::write(SoundBuffer *buffer)
{
    int32_t err;

    if (buffer) {
        err = snd_pcm_writei(alsa_handle, buffer->getData(), buffer->getPeriodSize());
        if (err != buffer->getPeriodSize()) {
            cout << "snd_pcm_writei: " << snd_strerror(err) << endl;
            exit(1);
        }
    }
    else {
        err = snd_pcm_writei(alsa_handle, silence->getData(), silence->getPeriodSize());
        if (err != silence->getPeriodSize()) {
            cout << "snd_pcm_writei: " << snd_strerror(err) << endl;
            exit(1);
        }
    }
}

void Alsa::close()
{
    snd_pcm_close (alsa_handle);
}

void Alsa::setupWithPulseAudio(int32_t rate, int32_t channels)
{
    const char device[] = "default";
    int32_t err;

    /* Open the ALSA audio device. Use default system sound output. */
    err = snd_pcm_open(&alsa_handle, device, SND_PCM_STREAM_PLAYBACK, 0);
    if (err < 0) {
        cout << "snd_pcm_open: " << snd_strerror(err) << endl;
        return;
    }
    err = snd_pcm_set_params(alsa_handle, SND_PCM_FORMAT_S16,
                             SND_PCM_ACCESS_RW_INTERLEAVED,
                             channels, rate, 0, 100000); /* 100ms latency */
    if (err < 0) {
        cout << "snd_pcm_set_params: " << snd_strerror(err) << endl;
        return;
    }
}

void Alsa::setupNoPulseAudio(int rate, int32_t channels, snd_pcm_uframes_t frames)
{
    const char *device="default";
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_sw_params_t *sw_params;
    int32_t err, dir;

    if ((err = snd_pcm_open (&alsa_handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        fprintf (stderr, "cannot open audio device %s (%s)\n",
                 device,
                 snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
        fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
                 snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_hw_params_any (alsa_handle, hw_params)) < 0) {
        fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
                 snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_hw_params_set_access (alsa_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        fprintf (stderr, "cannot set access type (%s)\n",
                 snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_hw_params_set_format (alsa_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
        fprintf (stderr, "cannot set sample format (%s)\n",
                 snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_hw_params_set_rate_near (alsa_handle, hw_params, (uint32_t *) &rate, &dir)) < 0) {
        fprintf (stderr, "cannot set sample rate (%s)\n",
                 snd_strerror (err));
        exit (1);
    }
    printf("sampling rate: %i\n", rate);

    if ((err = snd_pcm_hw_params_set_channels (alsa_handle, hw_params, channels)) < 0) {
        fprintf (stderr, "cannot set channel count (%s)\n",
                 snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_hw_params_set_period_size_near (alsa_handle, hw_params, &frames, &dir)) < 0) {
        fprintf (stderr, "cannot set period size (%s)\n",
                 snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_hw_params (alsa_handle, hw_params)) < 0) {
        fprintf (stderr, "cannot set parameters (%s)\n",
                 snd_strerror (err));
        exit (1);
    }

    snd_pcm_hw_params_free (hw_params);
}

int Alsa::avail()
{
    return snd_pcm_avail(alsa_handle);
}

int Alsa::delay()
{
    int32_t err;
    snd_pcm_sframes_t frames;
    if ((err = snd_pcm_delay(alsa_handle, &frames)) < 0) {
        cout << "snd_pcm_writei: " << snd_strerror (err) << endl;
        exit(1);
    }
    return frames;
}

} //end namespace output
} //end namespace aserver
