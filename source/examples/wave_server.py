#!/usr/bin/env python
# -*- coding:utf-8 -*-

import sys
import time
import socket

sys.path.append("..")
import bindings as aserver
import numpy as np


sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("0.0.0.0", 10101))
sock.setblocking(0)

period = 132    # 1.4 ms @ 44100 Hz

core = aserver.Core()
core.set_period(period)
core.set_processor()
core.alsa_output(dump_config=True, blocking=False)


gid = core.add_generator(aserver.GeneratorType.WAVE)
cfg = core.new_config("wave")
cfg.config.flags = aserver.WaveFlags("WAVE_INDEX PLAYBACK_COMMAND").value
# cfg.waveIndex = core.get_wave_index(filename)
cfg.waveIndex = core.add_wave(
    441, 1,
    (3.2e4 * np.cos(np.linspace(0, np.pi * 10, 441))).astype(np.int16)
)
cfg.command = aserver.PlaybackCommand.PLAY
core.add_source()


core.start_render_loop()

done = False
while not done:
    try:
        time.sleep(1e-6)
        try:
            data, addr = sock.recvfrom(1024)
        except:
            continue
        core.configure_generator(gid, cfg)
        print "play"
    except KeyboardInterrupt:
        done = True

print "done"

core.stop_render_loop()
core.shutdown()

