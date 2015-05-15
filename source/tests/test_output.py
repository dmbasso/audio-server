#!/usr/bin/env python
#-*- coding:utf-8 -*-

import os
import pytest

import bindings as aserver


default_path = "output.wav"
alt_path = "alternative.wav"

default_period = 2048
custom_period = 666


@pytest.fixture()
def core(request):
    return aserver.Core()


def test_file_default(core):
    core.set_processor()
    core.set_output(aserver.OutputType.FILE)
    core.render(1)
    core.shutdown()
    assert os.path.exists(default_path)
    os.unlink(default_path)


def test_file_configured(core):
    core.set_processor()
    core.set_output(aserver.OutputType.FILE)
    cfg = core.new_config("file")
    cfg.config.flags = aserver.OutputFlags("FILEPATH").value
    cfg.outputFilePath = alt_path
    core.configure_output(cfg)
    core.render(1)
    core.shutdown()
    assert os.path.exists(alt_path)
    os.unlink(alt_path)


@pytest.mark.parametrize("output", (
    aserver.OutputType.FILE, aserver.OutputType.MEMORY
))
@pytest.mark.parametrize("period", (default_period, custom_period))
def test_period(output, period, core):
    if period != default_period:
        core.set_period(period)
    core.set_processor()
    core.set_output(output)
    core.render(1)
    core.stop_output()
    rendered = core.get_output()
    core.shutdown()
    assert rendered.shape[0] == period
    # assert only silence was rendered:
    assert rendered[rendered != 0].shape[0] == 0


