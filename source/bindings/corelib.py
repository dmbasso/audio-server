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


# Core

class Core:
    def __init__(self):
        self.core = lib.new_core()

    # TODO: set_period_size

    def add_generator(self, generator_type):
        return lib.add_generator(self.core, generator_type)

    def configure_generator(self, gid, cfg):
        from ctypes import byref
        print cfg
        return lib.configure_generator(self.core, gid, byref(cfg))

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
    import coretypes as ct
    c.set_processor(ct.ProcessorType.DISTANCE_ATTENUATION)
    #c.set_processor(ProcessorType.ACOUSTICAVE)
    c.set_output(ct.OutputType.FILE)
    cfg = ct.PrimitiveConfigData()
    cfg.flags = 2
    cfg.frequency = 20
    #gid = c.add_generator(ct.GeneratorType.TEST)
    gid = c.add_generator(ct.GeneratorType.PRIMITIVE)
    c.configure_generator(gid, cfg);
    sid = c.add_source()  # RETURN VALUE NOT YET IMPLEMENTED
    c.render(200)
    c.shutdown()
