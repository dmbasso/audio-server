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


@pytest.fixture()
def core(request):
    retv = aserver.Core()
    retv.set_processor()    # NO_OPERATION
    retv.set_output()       # MEMORY

    def fin():
        retv.shutdown()

    request.addfinalizer(fin)
    return retv


def test_distance_attenuation(core):
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
    core.set_processor(aserver.ProcessorType.DISTANCE_ATTENUATION)

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

    energy_avgs = np.zeros([5, 2])
    distances = np.zeros([5, 2])
    distances[:, 0] = distances[:, 1] = np.arange(5, dtype=float) * 2
    ref_atten = 1 / (distances + 1)

    for i in range(n_positions):
        start = offset + period * n_periods * i
        end = start + measure_length
        segment = rendered[start:end, :]
        seg_energy_avg = np.average(energy(segment), axis=0)
        energy_avgs[i] = seg_energy_avg

    energy_avgs_norm = energy_avgs / energy_avgs[0, :]
    print energy_avgs_norm - ref_atten
    assert np.amax(energy_avgs_norm - ref_atten) < 0.05
