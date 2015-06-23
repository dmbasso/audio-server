# -*- coding:utf-8 -*-

import pytest
import numpy as np
from scipy.io import wavfile
from scipy.signal import hilbert
from matplotlib import pyplot as plt
import bindings as aserver


def energy(signal):
    natural = hilbert(signal)
    return np.abs(natural)


def write_test_wavefile(filename, srate=44100):
    wav = create_square_wave(srate)
    wavfile.write(filename, srate, wav)


def create_square_wave(length, srate=44100):
    period = srate / 220  # A3
    phases = np.arange(length) * np.pi / period
    wav = np.tanh(np.sin(phases) * 10)  # square the sin
    wav = wav / wav.max() * 32000
    return wav.astype(np.int16)


def create_noise(length, srate=44100):
    noise = np.random.random_integers(-32767, 32768, size=(length,))
    return noise.astype(np.int16)


@pytest.fixture()
def core(request):
    retv = aserver.Core()
    retv.set_processor()    # NO_OPERATION
    retv.set_output()       # MEMORY

    def fin():
        retv.shutdown()

    request.addfinalizer(fin)
    return retv


def test_set_geometry(core):
    core.set_processor(aserver.ProcessorType.ACOUSTICAVE)
    cfg = core.new_config("acousticave")
    cfg.config.flags = aserver.AcousticaveFlags("MODEL_FILEPATH").value
    handler = core._set_model_path(cfg, "model.obj")
    core.configure_processor(cfg)  # TODO: error processing, file not found
    del handler


@pytest.mark.parametrize("hrtf", (
    aserver.AcousticaveHRTF.MIT,
    aserver.AcousticaveHRTF.CIPIC,
    aserver.AcousticaveHRTF.LISTEN,
    aserver.AcousticaveHRTF.TUB,
    aserver.AcousticaveHRTF.IDENTITY
))
def test_distance_attenuation(core, hrtf):
    # core configuration
    period = 2048
    core.set_period(period)
    core.set_output(aserver.OutputType.MEMORY)

    # test configuration
    n_periods = 10
    n_positions = 5
    offset = 5 * period
    measure_length = 2 * period
    distance_offset = 0

    # processor configuration
    core.set_processor(aserver.ProcessorType.ACOUSTICAVE)
    cfg = core.new_config("acousticave")
    cfg.config.flags = aserver.AcousticaveFlags("HRTF").value
    cfg.hrtf = hrtf
    core.configure_processor(cfg)

    # wave generation
    length = 5 * 44100
    wav = create_square_wave(length)
    wid = core.add_wave(length, 1, wav)

    # generator configuration
    gid = core.add_generator(aserver.GeneratorType.WAVE)
    cfg = core.new_config("wave")
    cfg.config.flags = aserver.WaveFlags("WAVE_INDEX PLAYBACK_COMMAND").value
    cfg.waveIndex = wid
    cfg.command = aserver.PlaybackCommand.PLAY
    core.configure_generator(gid, cfg)

    # source configuration
    core.add_source()

    for i in range(n_positions):
        cfg = core.new_config("source")
        cfg.location = [0., distance_offset + i * 2., 0.]
        core.configure_source(gid, cfg)
        core.render(n_periods)

    core.stop_output()
    rendered = core.get_output().astype(float)

    en_avgs = np.zeros([5, 2])
    distances = np.zeros([5, 2])
    distances[:, 0] = distances[:, 1] = np.arange(5, dtype=float) * 2
    ref_atten = 1 / (distances + 1)

    for i in range(n_positions):
        start = offset + period * n_periods * i
        end = start + measure_length
        segment = rendered[start:end, :]
        seg_energy_avg = np.average(energy(segment), axis=0)
        en_avgs[i] = seg_energy_avg

    en_avgs_norm = en_avgs / en_avgs[0, :]
    print en_avgs_norm - ref_atten
    assert np.amax(en_avgs_norm - ref_atten) < 0.05


@pytest.mark.parametrize("hrtf", (
    aserver.AcousticaveHRTF.MIT,
    aserver.AcousticaveHRTF.CIPIC,
    aserver.AcousticaveHRTF.LISTEN,
    aserver.AcousticaveHRTF.TUB
))
def test_y_is_forward(core, hrtf):
    # core configuration
    period = 2048
    core.set_period(period)
    core.set_output(aserver.OutputType.MEMORY)

    # processor configuration
    core.set_processor(aserver.ProcessorType.ACOUSTICAVE)
    cfg = core.new_config("acousticave")
    cfg.config.flags = aserver.AcousticaveFlags("HRTF").value
    cfg.hrtf = hrtf
    core.configure_processor(cfg)

    # noise generation
    length = period * 9
    noise = create_noise(length)
    wid = core.add_wave(length, 1, noise)

    # generator configuration
    gid = core.add_generator(aserver.GeneratorType.WAVE)
    cfg = core.new_config("wave")
    cfg.config.flags = aserver.WaveFlags("WAVE_INDEX PLAYBACK_COMMAND").value
    cfg.waveIndex = wid
    cfg.command = aserver.PlaybackCommand.PLAY
    core.configure_generator(gid, cfg)

    # source configuration
    sid = core.add_source()
    cfg = core.new_config("source")
    cfg.location = [0., 10., 0.]
    core.configure_source(sid, cfg)

    core.render(10)

    cfg = core.new_config("wave")
    cfg.config.flags = aserver.WaveFlags("PLAYBACK_COMMAND").value
    cfg.command = aserver.PlaybackCommand.STOP
    core.configure_generator(gid, cfg)

    cfg = core.new_config("wave")
    cfg.config.flags = aserver.WaveFlags("WAVE_INDEX PLAYBACK_COMMAND").value
    cfg.waveIndex = wid
    cfg.command = aserver.PlaybackCommand.PLAY
    core.configure_generator(gid, cfg)

    # source configuration
    cfg = core.new_config("source")
    cfg.location = [0., -10., 0.]
    core.configure_source(sid, cfg)

    core.render(10)

    core.stop_output()
    rendered = core.get_output().astype(float)

    # test configuration
    n_periods = 10
    n_positions = 2
    offset = 4 * period
    measure_length = 5 * period
    en_avgs = np.zeros([2, 2])

    for i in range(n_positions):
        start = offset + period * n_periods * i
        end = start + measure_length
        segment = rendered[start:end, :]
        seg_energy_avg = np.average(energy(segment), axis=0)
        en_avgs[i] = seg_energy_avg

    print en_avgs
    lr_en = en_avgs / np.amax(en_avgs, axis=1).reshape(n_positions, 1)

    fb_en = en_avgs / np.amax(en_avgs, axis=0)
    print lr_en
    print fb_en

    # the difference between the left and right channel energy in both
    # positions cannot be significant (ths = 0.1)
    assert np.absolute(lr_en[0, 0] - lr_en[0, 1]) < 0.1
    assert np.absolute(lr_en[1, 0] - lr_en[1, 1]) < 0.1

    # the front position must have higher energy avg than the back position
    assert fb_en[0, 0] > fb_en[1, 0]
    assert fb_en[0, 1] > fb_en[1, 1]


@pytest.mark.parametrize("hrtf", (
    aserver.AcousticaveHRTF.MIT,
    aserver.AcousticaveHRTF.CIPIC,
    aserver.AcousticaveHRTF.LISTEN,
    aserver.AcousticaveHRTF.TUB
))
def test_x_is_right(core, hrtf):
    # core configuration
    period = 2048
    core.set_period(period)
    core.set_output(aserver.OutputType.MEMORY)

    # processor configuration
    core.set_processor(aserver.ProcessorType.ACOUSTICAVE)
    cfg = core.new_config("acousticave")
    cfg.config.flags = aserver.AcousticaveFlags("HRTF").value
    cfg.hrtf = hrtf
    core.configure_processor(cfg)

    # noise generation
    length = period * 9
    noise = create_noise(length)
    wid = core.add_wave(length, 1, noise)

    # generator configuration
    gid = core.add_generator(aserver.GeneratorType.WAVE)
    cfg = core.new_config("wave")
    cfg.config.flags = aserver.WaveFlags("WAVE_INDEX PLAYBACK_COMMAND").value
    cfg.waveIndex = wid
    cfg.command = aserver.PlaybackCommand.PLAY
    core.configure_generator(gid, cfg)

    # source configuration
    sid = core.add_source()
    cfg = core.new_config("source")
    cfg.location = [10., 0., 0.]
    core.configure_source(sid, cfg)

    core.render(10)

    cfg = core.new_config("wave")
    cfg.config.flags = aserver.WaveFlags("PLAYBACK_COMMAND").value
    cfg.command = aserver.PlaybackCommand.STOP
    core.configure_generator(gid, cfg)

    cfg = core.new_config("wave")
    cfg.config.flags = aserver.WaveFlags("WAVE_INDEX PLAYBACK_COMMAND").value
    cfg.waveIndex = wid
    cfg.command = aserver.PlaybackCommand.PLAY
    core.configure_generator(gid, cfg)

    # source configuration
    cfg = core.new_config("source")
    cfg.location = [-10., 0., 0.]
    core.configure_source(sid, cfg)

    core.render(10)

    core.stop_output()
    rendered = core.get_output().astype(float)

    # test configuration
    n_periods = 10
    n_positions = 2
    offset = 4 * period
    measure_length = 5 * period
    en_avgs = np.zeros([2, 2])

    for i in range(n_positions):
        start = offset + period * n_periods * i
        end = start + measure_length
        segment = rendered[start:end, :]
        seg_energy_avg = np.average(energy(segment), axis=0)
        en_avgs[i] = seg_energy_avg

    lr_en = en_avgs / np.amax(en_avgs, axis=1).reshape(n_positions, 1)
    fb_en = en_avgs / np.amax(en_avgs, axis=0)

    print en_avgs
    print lr_en
    print fb_en

    # the channel closest to the listener position must have higher energy avg
    # the threshold for the furthest ear is ths = 0.25
    assert np.absolute(lr_en[0, 1] - lr_en[0, 0]) > 0.75
    assert np.absolute(lr_en[1, 0] - lr_en[1, 1]) > 0.75

    # each channel, in the oposing position, must have significantly less
    # energy avg (ths = 0.25)
    assert fb_en[0, 1] - fb_en[1, 1] > 0.75
    assert fb_en[1, 0] - fb_en[0, 0] > 0.75


@pytest.mark.parametrize("hrtf", (
    aserver.AcousticaveHRTF.MIT,
    aserver.AcousticaveHRTF.CIPIC,
    aserver.AcousticaveHRTF.LISTEN,
    aserver.AcousticaveHRTF.TUB
))
def test_z_is_up(core, hrtf):
    # core configuration
    period = 2048
    core.set_period(period)
    core.set_output(aserver.OutputType.MEMORY)

    # processor configuration
    core.set_processor(aserver.ProcessorType.ACOUSTICAVE)
    cfg = core.new_config("acousticave")
    cfg.config.flags = aserver.AcousticaveFlags("HRTF").value
    cfg.hrtf = hrtf
    core.configure_processor(cfg)

    # noise generation
    length = period * 9
    noise = create_noise(length)
    wid = core.add_wave(length, 1, noise)

    # generator configuration
    gid = core.add_generator(aserver.GeneratorType.WAVE)
    cfg = core.new_config("wave")
    cfg.config.flags = aserver.WaveFlags("WAVE_INDEX PLAYBACK_COMMAND").value
    cfg.waveIndex = wid
    cfg.command = aserver.PlaybackCommand.PLAY
    core.configure_generator(gid, cfg)

    # source configuration
    sid = core.add_source()
    cfg = core.new_config("source")
    cfg.location = [0., 0., 10.]
    core.configure_source(sid, cfg)

    core.render(10)

    cfg = core.new_config("wave")
    cfg.config.flags = aserver.WaveFlags("PLAYBACK_COMMAND").value
    cfg.command = aserver.PlaybackCommand.STOP
    core.configure_generator(gid, cfg)

    cfg = core.new_config("wave")
    cfg.config.flags = aserver.WaveFlags("WAVE_INDEX PLAYBACK_COMMAND").value
    cfg.waveIndex = wid
    cfg.command = aserver.PlaybackCommand.PLAY
    core.configure_generator(gid, cfg)

    cfg = core.new_config("acousticave")
    cfg.config.flags = aserver.AcousticaveFlags.AAVE_LISTENER_ORIENTATION
    cfg.orientation = [0., np.pi / 2., - np.pi / 2.]
    core.configure_processor(cfg)

    core.render(10)

    core.stop_output()
    rendered = core.get_output().astype(float)

    # test configuration
    n_periods = 10
    n_positions = 2
    offset = 4 * period
    measure_length = 5 * period
    en_avgs = np.zeros([2, 2])

    for i in range(n_positions):
        start = offset + period * n_periods * i
        end = start + measure_length
        segment = rendered[start:end, :]
        seg_energy_avg = np.average(energy(segment), axis=0)
        en_avgs[i] = seg_energy_avg

    lr_en = en_avgs / np.amax(en_avgs, axis=1).reshape(n_positions, 1)
    fb_en = en_avgs / np.amax(en_avgs, axis=0)

    print en_avgs
    print lr_en
    print fb_en

    plt.plot(rendered)
    plt.show()

    # the channel closest to the listener position must have higher energy avg
    # the threshold for the furthest ear is ths = 0.25
    assert np.absolute(lr_en[0, 1] - lr_en[0, 0]) > 0.75
    assert np.absolute(lr_en[1, 0] - lr_en[1, 1]) > 0.75

    # each channel, in the oposing position, must have significantly less
    # energy avg (ths = 0.25)
    assert fb_en[0, 1] - fb_en[1, 1] > 0.75
    assert fb_en[1, 0] - fb_en[0, 0] > 0.75
