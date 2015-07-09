#!/usr/bin/env python
# -*- coding:utf-8 -*-

"""
    Example of low latency ALSA output.

    alsa_output         # continuos sine of varying frequency 220 - 880
    alsa_output 0       # makes 5 Hz pulses 2.9 ms wide
    alsa_output 0 10    # makes 10 Hz pulses 2.9 ms wide
"""

import sys
import math

sys.path.append("..")
import bindings as aserver


period = 130    # 2.9 ms @ 44100 Hz

continuos = int(sys.argv[1]) if len(sys.argv) > 1 else 1
act_freq = int(sys.argv[2]) if len(sys.argv) > 2 else 5
activation_period = 44100 / period / act_freq


core = aserver.Core()
core.set_period(period)
core.set_processor()
core.alsa_output(dump_config=True)

gid = core.add_generator(aserver.GeneratorType.PRIMITIVE)
cfg = core.new_config("primitive")
cfg.config.flags = aserver.PrimitiveFlags.FREQUENCY

cfg2 = core.new_config("primitive")
cfg2.config.flags = aserver.PrimitiveFlags.PRIMITIVE_AMPLITUDE

core.add_source()

done = False
ang = 0
cnt = 0
while not done:
    core.render(1)
    try:
        freq = int(440 * (1.25 + .75 * math.cos(ang)))
        if not continuos:
            if not cnt:
                print freq
                cfg2.amplitude = 1
                core.configure_generator(gid, cfg2)
            else:
                if cnt == 1:
                    cfg2.amplitude = 0
                    core.configure_generator(gid, cfg2)
            cnt = (cnt + 1) % activation_period
        cfg.frequency = freq
        core.configure_generator(gid, cfg)
        ang += math.pi / (44100 / period)
    except KeyboardInterrupt:
        done = True

core.stop_output()
core.shutdown()

