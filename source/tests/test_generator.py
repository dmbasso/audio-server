# -*- coding:utf-8 -*-

import pytest
import numpy as np
from scipy.io import wavfile
from scipy.signal import hilbert

import bindings as aserver


def energy(signal):
    natural = hilbert(signal)
    return np.abs(natural)


def fundamental_frequency(signal):
    corr = np.convolve(signal, signal[::-1])
    corr = corr[corr.shape[0] / 2:]
    diff = np.diff(corr)
    neg_peak = (diff > 0).nonzero()[0][0]
    peak = corr[neg_peak:].argmax()
    return 44100. / (peak + neg_peak)


def write_test_wavefile(filename, srate=44100):
    length = srate  # 1s
    period = srate / 220  # A3
    phases = np.arange(length) * np.pi / period
    wav = np.tanh(np.sin(phases) * 10)  # square the sin
    wav = wav / wav.max() * 32000
    wavfile.write(filename, srate, wav.astype(np.int16))
    return wav


@pytest.fixture()
def core(request):
    retv = aserver.Core()
    retv.set_processor()    # NO_OPERATION
    retv.set_output()       # MEMORY

    def fin():
        retv.shutdown()

    request.addfinalizer(fin)
    return retv


@pytest.mark.parametrize("waveform", (
    aserver.WaveformType.SINE,
    aserver.WaveformType.SQUARE,
    aserver.WaveformType.SAWTOOTH,
))
def test_primitive(core, waveform):
    gid = core.add_generator(aserver.GeneratorType.PRIMITIVE)
    cfg = core.new_config("primitive")
    cfg.config.flags = aserver.PrimitiveFlags.WAVEFORM
    cfg.waveform = waveform
    core.configure_generator(gid, cfg)
    core.add_source()

    core.render(1)
    core.stop_output()
    rendered = core.get_output().astype(float)
    assert all(rendered[:, 0] == rendered[:, 1])
    assert abs(fundamental_frequency(rendered[:, 0]) - 220) < 2
    instant_energy = energy(rendered[:, 0])
    l = instant_energy.shape[0]
    # cut the endings (signal is periodic)
    mean_energy = instant_energy[l / 8:l / 8 * 6].mean()
    assert mean_energy > 22800  # minimum mean energy (sawtooth)


def test_wave(core):
    filename = "/tmp/test_wave.wav"
    data = write_test_wavefile(filename)
    gid = core.add_generator(aserver.GeneratorType.WAVE)
    cfg = core.new_config("wave")
    cfg.config.flags = aserver.WaveFlags("WAVE_INDEX PLAYBACK_COMMAND").value
    cfg.waveIndex = core.get_wave_index(filename)
    cfg.command = aserver.PlaybackCommand.PLAY
    core.configure_generator(gid, cfg)
    core.add_source()

    core.render(1)
    core.stop_output()
    rendered = core.get_output().astype(np.int16)
    assert all(rendered[:, 0] == rendered[:, 1])  # mono in two channels
    # assert the rendered data is exactly equal to the original wave
    assert all(rendered[:, 0] == data[:rendered.shape[0]].astype(np.int16))


def test_wave_playback_commands(core):
    filename = "/tmp/test_wave.wav"
    data = write_test_wavefile(filename).astype(np.int16)
    gid = core.add_generator(aserver.GeneratorType.WAVE)
    cfg = core.new_config("wave")
    cfg.config.flags = aserver.WaveFlags.WAVE_INDEX
    cfg.waveIndex = core.get_wave_index(filename)
    core.configure_generator(gid, cfg)
    core.add_source()

    cfg.config.flags = aserver.WaveFlags.PLAYBACK_COMMAND
    core.render(1)  # silence
    cfg.command = aserver.PlaybackCommand.PLAY
    core.configure_generator(gid, cfg)
    core.render(1)  # test_wave
    cfg.command = aserver.PlaybackCommand.PAUSE
    core.configure_generator(gid, cfg)
    core.render(1)  # silence
    cfg.command = aserver.PlaybackCommand.PLAY
    core.configure_generator(gid, cfg)
    core.render(1)  # test_wave continued
    cfg.command = aserver.PlaybackCommand.STOP
    core.configure_generator(gid, cfg)
    cfg.command = aserver.PlaybackCommand.PLAY
    core.configure_generator(gid, cfg)
    core.render(1)  # test_wave from the beginning

    core.stop_output()
    rendered = core.get_output().astype(np.int16)

    # silences
    assert all(rendered[:2048, 0] == 0)
    assert all(rendered[4096:4096 + 2048, 0] == 0)
    # assert the non silence parts
    assert all(rendered[2048:4096, 0] == data[:2048])
    assert all(rendered[4096 + 2048:4096 + 4096, 0] == data[2048:4096])
    assert all(rendered[4096 + 4096:, 0] == data[:2048])


@pytest.mark.skipif(True, reason="Script::render segfaults")
def test_script_add_keyframes(core):
    gid = core.add_generator(aserver.GeneratorType.SCRIPT)
    cfg, keyframes = core.new_keyframes(5)
    for i in range(5):
        keyframes[i].start = i
        keyframes[i].location[0] = i
        keyframes[i].wave.waveIndex = i
    core.configure_generator(gid, cfg)
    core.add_source()

    core.render(1)
    core.stop_output()
    rendered = core.get_output().astype(np.int16)
    assert all(rendered[:, 0] == 0)  # all silence because of invalid waves
