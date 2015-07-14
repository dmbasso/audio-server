#include <iostream>
#include <unistd.h>
#include "alsa.h"

using namespace std;

namespace aserver {
namespace output {

Alsa::Alsa(Core *core, uint32_t periodSize) : Output(core, periodSize)
{
}

void Alsa::config(ConfigData *configData)
{
    AlsaOutputConfigData *cfgData = (AlsaOutputConfigData *) configData;

    if (cfgData->flags & alsaConfigFlags::RATE) {
        rate = cfgData->rate;
    } else {
        rate = 44100;
    }
    if (cfgData->flags & alsaConfigFlags::CHANNELS) {
        channels = cfgData->channels;
    } else {
        channels = 2;
    }
    blocking = cfgData->flags & alsaConfigFlags::BLOCKING;
    setup();
    if (cfgData->flags & alsaConfigFlags::DUMP_CONFIG) {
        snd_output_t *output = NULL;
        snd_output_stdio_attach(&output, stdout, 0);
        snd_pcm_dump(alsa_handle, output);
        snd_output_close(output);
    }
}

void Alsa::write(SoundBuffer *buffer)
{
    int32_t err;
    auto period = (buffer ? buffer : silence)->getPeriodSize();

    while (!blocking and avail() < period * 1.3) {
        usleep(1);
    }

    if (buffer) {
        err = snd_pcm_writei(alsa_handle, buffer->getData(), buffer->getPeriodSize());
        if (err != buffer->getPeriodSize()) {
            cout << "Alsa::write buffer: " << snd_strerror(err) << endl;
            exit(1);
        }
    } else {
        err = snd_pcm_writei(alsa_handle, silence->getData(), silence->getPeriodSize());
        if (err != silence->getPeriodSize()) {
            cout << "Alsa::write silence: " << snd_strerror(err) << endl;
            exit(1);
        }
    }
}

void Alsa::close()
{
    if (alsa_handle) {
        snd_pcm_close(alsa_handle);
        alsa_handle = nullptr;
    }
}

void Alsa::setup()
{
    const char *device = "hw:0,0";  // device "default" is unusable
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_sw_params_t *sw_params;
    int32_t err, dir = 0;
    int resample = 1;
    snd_pcm_uframes_t frames = silence->getPeriodSize();

    if ((err = snd_pcm_open (
            &alsa_handle,
            device,
            SND_PCM_STREAM_PLAYBACK,
            blocking ? 0 : SND_PCM_NONBLOCK
         )) < 0) {
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

    if ((err = snd_pcm_hw_params_set_rate_resample(alsa_handle, hw_params, resample)) < 0) {
        fprintf(stderr, "Resample setup failed for %s (val %i): %s\n", device, resample, snd_strerror(err));
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

    if ((err = snd_pcm_hw_params_set_rate_near (alsa_handle, hw_params, &rate, &dir)) < 0) {
        fprintf (stderr, "cannot set sample rate (%s)\n",
                 snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_hw_params_set_channels (alsa_handle, hw_params, channels)) < 0) {
        fprintf (stderr, "cannot set channel count (%s)\n",
                 snd_strerror (err));
        exit (1);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

    snd_pcm_uframes_t buffersize = frames * 2;
    if ((err = snd_pcm_hw_params_set_buffer_size_near(alsa_handle, hw_params, &buffersize)) < 0) {
        fprintf (stderr, "cannot set buffer size (%s)\n",
                 snd_strerror (err));
        exit (1);
    }

    if ((err = snd_pcm_hw_params_set_period_size_near (alsa_handle, hw_params, &frames, &dir)) < 0) {
        fprintf (stderr, "cannot set period size (%s)\n",
                 snd_strerror (err));
        exit (1);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

    if ((err = snd_pcm_hw_params (alsa_handle, hw_params)) < 0) {
        fprintf (stderr, "cannot set parameters (%s)\n",
                 snd_strerror (err));
        exit (1);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

    if ((err = snd_pcm_sw_params_malloc (&sw_params)) < 0) {
        fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
                 snd_strerror (err));
        exit (1);
    }

    err = snd_pcm_sw_params_current (alsa_handle, sw_params);
    if (err < 0) {
        printf ("Unable to determine current sw_params: %s\n", snd_strerror (err));
        return;
    }

    err = snd_pcm_sw_params_set_start_threshold (alsa_handle, sw_params, 0);
    if (err < 0) {
        printf ("Unable to set start threshold mode: %s\n", snd_strerror (err));
        return;
    }

    // no xruns
    err = snd_pcm_sw_params_set_stop_threshold (alsa_handle, sw_params, buffersize * 2);
    if (err < 0) {
        printf ("Unable to set stop threshold mode: %s\n", snd_strerror (err));
        return;
    }

    err = snd_pcm_sw_params_set_avail_min (alsa_handle, sw_params, 4);
    if (err < 0) {
        printf ("Unable to set avail min: %s\n", snd_strerror (err));
        return;
    }

    err = snd_pcm_sw_params (alsa_handle, sw_params);
    if (err < 0) {
        printf ("Unable to set sw params: %s\n", snd_strerror (err));
        return;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

    snd_pcm_hw_params_free (hw_params);
    snd_pcm_sw_params_free (sw_params);
}

int Alsa::avail()
{
    int retv = snd_pcm_avail(alsa_handle);
    if (retv < 0) {
        printf ("Alsa::avail() error: %s\n", snd_strerror (retv));
        exit (1);
    }
    return retv;
}

int Alsa::delay()
{
    int32_t err;
    snd_pcm_sframes_t frames;
    if ((err = snd_pcm_delay(alsa_handle, &frames)) < 0) {
        cout << "Alsa::delay() error: " << snd_strerror (err) << endl;
        exit(1);
    }
    return frames;
}

} //end namespace output
} //end namespace aserver
