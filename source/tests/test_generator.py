# -*- coding:utf-8 -*-

import os
import pytest
import numpy as np
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


@pytest.fixture()
def core(request):
    retv = aserver.Core()
    retv.set_processor()    # NO_OPERATION
    # retv.set_output()       # MEMORY
    # TODO: use memory after it is fixed
    retv.set_output(aserver.OutputType.FILE)

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
    assert mean_energy > 23000  # minimum mean energy (sawtooth)

