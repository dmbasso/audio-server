# -*- coding:utf-8 -*-

import os
import subprocess
from cffi import FFI, __version_info__ as cffi_version

from . import enums


if cffi_version < (0, 9, 0):
    raise RuntimeError("""
        The installed CFFI library is too old, try upgrading with:
        sudo pip install --upgrade cffi
    """)


path = os.path.abspath(os.path.dirname(__file__))
header = os.path.join(path, 'cshim.h')

ffi = FFI()

ffi.cdef(
    subprocess.Popen(
        ['cc', '-E', '-DFOR_FFI', header],
        stdout=subprocess.PIPE
    ).communicate()[0],
    packed=True
)

lib = ffi.dlopen(os.path.join(path, 'libcshim.so'))


# Core

class Core:
    def __init__(self):
        self.core = lib.new_core()
        self.output_path = "output.wav"

    def set_period(self, size):
        return lib.set_period(self.core, size)

    def get_wave_index(self, filename):
        return lib.get_wave_index(self.core, filename)

    def add_wave(self, size, channels, data):
        ptr = ffi.from_buffer(data)
        return lib.add_wave(self.core, size, channels, ptr)

    def new_config(self, cfg_type):
        return ffi.new(cfg_type + "_cfg_t *")

    def add_generator(self, generator_type):
        return lib.add_generator(self.core, generator_type)

    def configure_generator(self, gid, cfg):
        return lib.configure_generator(
            self.core, gid, ffi.cast("generator_cfg_t *", cfg)
        )

    def new_keyframes(self, count):
        cfg = self.new_config("script")
        cfg.config.flags = enums.ScriptFlags.COMMAND
        cfg.command = enums.ScriptCommand.ADD_KEYFRAMES
        cfg.keyframeCount = count
        cfg.keyframes = keyframes = ffi.new("keyframe_t[]", count)
        return cfg, keyframes

    def add_source(self):
        return lib.add_source(self.core)

    def configure_source(self, gid, cfg):
        return lib.configure_source(
            self.core, gid, ffi.cast("source_cfg_t *", cfg)
        )

    def set_source_generator(self, gid, sid):
        return lib.set_source_generator(self.core, sid, gid)

    def set_processor(self, processor_type=enums.ProcessorType.NO_OPERATION):
        lib.set_processor(self.core, processor_type)

    def configure_processor(self, cfg):
        return lib.configure_processor(
            self.core, ffi.cast("processor_cfg_t *", cfg)
        )

    def set_output(self, output_type=enums.OutputType.MEMORY):
        self.output_type = output_type
        lib.set_output(self.core, output_type)

    def configure_output(self, cfg):
        if self.output_type == enums.OutputType.FILE:
            self.output_path = cfg.outputFilePath
        return lib.configure_output(
            self.core, ffi.cast("output_cfg_t *", cfg)
        )

    def get_output(self):
        if self.output_type == enums.OutputType.FILE:
            from scipy.io import wavfile
            srate, signal = wavfile.read(self.output_path)
            return signal
        elif self.output_type == enums.OutputType.MEMORY:
            import numpy as np
            size = ffi.new("uint64_t *")
            mem = lib.get_output(self.core, size)
            mem2 = str(ffi.buffer(mem, size[0] * 2))
            signal = np.fromstring(mem2, dtype=np.int16)
            lib.free_output(self.core, mem)
            return signal.reshape((-1, 2))
        else:
            raise RuntimeError("the selected output does not collect audio")

    def render(self, periods=1):
        lib.render(self.core, periods)

    def stop_output(self):
        lib.stop_output(self.core)

    def shutdown(self):
        lib.del_core(self.core)
        self.core = None

    def reset(self, reset_output):
        lib.reset(self.core, reset_output)
