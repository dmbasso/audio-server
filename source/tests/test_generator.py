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


def test_wave_with_small_buffer(core):
    buff = np.sin(np.linspace(0, np.pi * 4, 100)) * 32000
    buff = buff.astype(np.int16)
    wid = core.add_wave(100, 1, buff)
    buff2 = np.zeros(1500, dtype=np.int16)
    buff2[1000] = 12345
    wid2 = core.add_wave(1500, 1, buff2)

    gid = core.add_generator(aserver.GeneratorType.WAVE)
    cfg = core.new_config("wave")
    cfg.config.flags = aserver.WaveFlags("WAVE_INDEX PLAYBACK_COMMAND").value
    cfg.waveIndex = wid
    cfg.command = aserver.PlaybackCommand.PLAY
    core.configure_generator(gid, cfg)
    core.add_source()

    gid = core.add_generator(aserver.GeneratorType.WAVE)
    cfg.waveIndex = wid2
    core.configure_generator(gid, cfg)
    core.add_source()

    core.render(1)
    core.stop_output()
    rendered = core.get_output().astype(np.int16)
    assert all(rendered[:, 0] == rendered[:, 1])  # mono in two channels
    assert all(rendered[:100, 0] == buff)
    assert all(rendered[100:1000, 0] == 0)
    assert all(rendered[1001:, 0] == 0)
    assert rendered[1000, 0] == 12345


def test_script_precision(core):
    buff = np.ones(1, dtype=np.int16)
    wid = core.add_wave(1, 1, buff)
    gid = core.add_generator(aserver.GeneratorType.SCRIPT)
    cfg, keyframes = core.new_keyframes(5)
    for i in range(5):
        keyframes[i].wave.config.flags = aserver.WaveFlags.PLAYBACK_COMMAND
        if not i:
            keyframes[i].wave.config.flags |= aserver.WaveFlags.WAVE_INDEX
            keyframes[i].wave.waveIndex = wid
        keyframes[i].start = 2 * (1 + i)
        keyframes[i].wave.command = aserver.PlaybackCommand.PLAY
    core.configure_generator(gid, cfg)

    cfg.command = aserver.ScriptCommand.PLAY
    core.configure_generator(gid, cfg)
    core.add_source()
    core.render(1)
    core.stop_output()
    rendered = core.get_output().astype(np.int16)
    should_be = np.zeros_like(rendered)

    for i in range(5):
        should_be[44100 * (i + 1) * 2 / 1000] = 1

    assert all(should_be[:, 0] == rendered[:, 0])
    assert all(should_be[:, 1] == rendered[:, 1])


@pytest.mark.parametrize("wavelength", (220, 441, 661))
@pytest.mark.parametrize("offset", (-1, 0, 1))
def test_script_boundaries(core, wavelength, offset):
    period_size = 441
    render_periods = 3
    should_be = np.zeros([period_size * render_periods, 2])
    gid = core.add_generator(aserver.GeneratorType.SCRIPT)

    if (offset == -1):
        n_keyframes = 2
    else:
        n_keyframes = 3

    cfg, keyframes = core.new_keyframes(n_keyframes)

    kf_index = 0
    for i in range(3):
        # calculate keyframe onset
        start = offset + i * 10

        if start < 0:
            continue

        # generate and add waves
        wave = np.random.uniform(-32000, 32000, wavelength).astype(np.int16)
        wid = core.add_wave(wavelength, 1, wave)

        # generate and add keyframes
        flags = aserver.WaveFlags("PLAYBACK_COMMAND WAVE_INDEX").value
        keyframes[kf_index].wave.config.flags = flags
        keyframes[kf_index].wave.waveIndex = wid
        keyframes[kf_index].start = start
        keyframes[kf_index].wave.command = aserver.PlaybackCommand.PLAY
        kf_index += 1

        # generate the expected output
        start *= 44.1
        view = should_be[start:start + wavelength]
        view[:, 0] = wave[:view.shape[0]]
        view[:, 1] = wave[:view.shape[0]]
    core.configure_generator(gid, cfg)

    cfg.command = aserver.ScriptCommand.PLAY
    core.configure_generator(gid, cfg)
    core.add_source()

    core.set_period(period_size)
    core.render(3)
    core.stop_output()
    rendered = core.get_output().astype(np.int16)

    assert all(should_be[:, 0] == rendered[:, 0])
    assert all(should_be[:, 1] == rendered[:, 1])


@pytest.mark.parametrize("delay", (-11, -1, 0, 1, 11))
def test_script_delay(core, delay):
    period_size = 441
    render_periods = 6
    wavelength = 220
    should_be = np.zeros([period_size * render_periods, 2])
    gid = core.add_generator(aserver.GeneratorType.SCRIPT)
    n_keyframes = 5
    cfg, keyframes = core.new_keyframes(n_keyframes)

    for i in range(n_keyframes):
        # generate and add waves
        wave = np.random.uniform(-32000, 32000, wavelength).astype(np.int16)
        wid = core.add_wave(wavelength, 1, wave)

        # calculate keyframe onset
        start = i * 10

        # generate and add keyframes
        flags = aserver.WaveFlags("PLAYBACK_COMMAND WAVE_INDEX").value
        keyframes[i].wave.config.flags = flags
        keyframes[i].wave.waveIndex = wid
        keyframes[i].start = start
        keyframes[i].wave.command = aserver.PlaybackCommand.PLAY

        # generate the expected output
        start += delay
        if start >= 0:
            start = start * 44100 // 1000
            view = should_be[start:start + wavelength]
            view[:, 0] = wave[:view.shape[0]]
            view[:, 1] = wave[:view.shape[0]]
    core.configure_generator(gid, cfg)

    cfg.command = aserver.ScriptCommand.PLAY
    cfg.delay = delay
    core.configure_generator(gid, cfg)
    core.add_source()

    core.set_period(period_size)
    core.render(render_periods)
    core.stop_output()
    rendered = core.get_output().astype(np.int16)

    assert all(should_be[:, 0] == rendered[:, 0])
    assert all(should_be[:, 1] == rendered[:, 1])
