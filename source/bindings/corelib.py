# -*- coding:utf-8 -*-

import os
import subprocess
from cffi import FFI

path = os.path.abspath(os.path.dirname(__file__))
header = os.path.join(path, 'cshim.h')

ffi = FFI()

ffi.cdef(
    subprocess.Popen(
        ['cc', '-E', '-DFOR_FFI', header],
        stdout=subprocess.PIPE
    ).communicate()[0]
)

lib = ffi.dlopen(os.path.join(path, 'libcshim.so'))


# Generators

class GeneratorType:
    PRIMITIVE = 1
    WAVE = 2
    TEST = 3
    SCRIPT = 4


class WaveformType:
    SINE = 1
    SQUARE = 2
    SAWTOOTH = 3


class PlaybackCommand:
    PLAY = 1
    STOP = 2
    PAUSE = 3
    PLAY_LOOP = 4
    REVERSE = 5


class PlaybackState:
    PLAYING = 1
    STOPPED = 2
    PAUSED = 3
    REWINDING = 4


# Processors

class ProcessorType:
    NO_OPERATION = 1
    ACOUSTICAVE = 2
    DISTANCE_ATTENUATION = 3


# Output

class OutputType:
    FILE = 1
    ALSA = 2
    MEMORY = 3


# Core

class Core:
    def __init__(self):
        self.core = lib.new_core()

    # TODO: set_period_size

    def add_generator(self, generator_type):
        return lib.add_generator(self.core, generator_type)

    # int configure_generator(core_t* core, int gid, cfg)

    def add_source(self):
        return lib.add_source(self.core)

    # int configure_source(core_t* core, int sid, source_config_data_t* cfg)

    def set_processor(self, processor_type):
        lib.set_processor(self.core, processor_type)

    # int configure_processor(core_t* core, processor_config_data_t* cfg);

    def set_output(self, output_type):
        lib.set_output(self.core, output_type)

    # int configure_output(core_t* core, output_config_data_t* cfg);

    def render(self, periods=1):
        lib.render(self.core, periods)

    def shutdown(self):
        lib.del_core(self.core)


if __name__ == '__main__':
    c = Core()
    # c.set_processor(ProcessorType.DISTANCE_ATTENUATION)
    c.set_processor(ProcessorType.ACOUSTICAVE)
    c.set_output(OutputType.FILE)
    gid = c.add_generator(GeneratorType.TEST)
    sid = c.add_source()  # RETURN VALUE NOT YET IMPLEMENTED
    c.render(200)
    c.shutdown()
