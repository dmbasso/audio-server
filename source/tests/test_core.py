# -*- coding:utf-8 -*-

import os
import pytest
import numpy as np
import random

from matplotlib import pyplot as plt
import bindings as aserver


@pytest.fixture()
def core(request):
    retv = aserver.Core()
    retv.set_processor()    # NO_OPERATION
    retv.set_output()       # MEMORY

    def fin():
        retv.shutdown()

    request.addfinalizer(fin)
    return retv


@pytest.mark.parametrize("processor", (
    aserver.ProcessorType.NO_OPERATION,
    aserver.ProcessorType.ACOUSTICAVE,
    aserver.ProcessorType.DISTANCE_ATTENUATION
))
def test_set_processor(core, processor):
    period1 = 2048
    period2 = 4196
    core.set_period(period1)
    core.set_output(aserver.OutputType.MEMORY)
    core.set_processor(processor)
    core.add_generator(aserver.GeneratorType.PRIMITIVE)
    core.add_source()
    core.render(1)

    core.set_processor(processor)
    core.set_period(period2)
#    core.add_generator(aserver.GeneratorType.PRIMITIVE)
#    core.add_source()
    core.render(1)

    core.stop_output()
    rendered = core.get_output()  # .astype(float)

    # assert something other than silence was rendered:
    assert rendered[rendered != 0].shape[0] != 0
    # assert the correct number ofs samples rendered
    assert rendered.shape[0] == period1 + period2

    # plot the output
#    plt.plot(rendered)
#    plt.show()


#@pytest.mark.parametrize("generator", (
#    aserver.GeneratorType.PRIMITIVE, aserver.GeneratorType.WAVE,
#    aserver.GeneratorType.TEST, aserver.GeneratorType.SCRIPT,
#    aserver.GeneratorType.NOISE
#))
def test_add_generator(core):
    gid = core.add_generator(aserver.GeneratorType.NOISE)
    assert gid == 0
    gid = core.add_generator(-666)
    assert gid == -1

    core.reset(False)

    gid = core.add_generator(-1)
    gid = core.add_generator(aserver.GeneratorType.PRIMITIVE)
    assert gid == 0
    gid = core.add_generator(aserver.GeneratorType.WAVE)
    gid = core.add_generator(-1)
    gid = core.add_generator(aserver.GeneratorType.TEST)
    gid = core.add_generator(aserver.GeneratorType.SCRIPT)
    assert gid == 3


def test_set_period(core):
    periods = np.random.randint(1, 4196, 50)
    core.set_output(aserver.OutputType.MEMORY)
    #core.set_processor()
    core.set_processor(aserver.ProcessorType.ACOUSTICAVE)

    for p in periods:
        core.set_period(p)
        core.render(1)

    core.stop_output()
    rendered = core.get_output()  # .astype(float)
#    core.shutdown()

    # assert only silence was rendered:
    assert rendered[rendered != 0].shape[0] == 0
    # assert the correct number of samples rendered
    assert rendered.shape[0] == sum(periods)

    # plot the output
#    plt.plot(rendered)
#    plt.show()


def test_add_source():
    core = aserver.Core()
#    core.set_output(aserver.OutputType.MEMORY)
#    core.set_processor()
    sid = core.add_source()
    assert sid == -1

    core.set_processor()

    for i in np.arange(20):
        sid = core.add_source()
        assert sid == i

    core.shutdown()


#def test_set_output():
#
#    core.set_output()



#def test_set_source_generator():

#    core.add_source()


#def test_render():

#    core.render(123123)
