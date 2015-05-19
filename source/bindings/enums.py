# -*- coding:utf-8 -*-

"""
    Enumerations and other declarations
"""


class ConfigFlags:
    def __init__(self, flags):
        self.value = sum(getattr(self, flag) for flag in flags.split())


# Generators


class GeneratorType:
    PRIMITIVE = 1
    WAVE = 2
    TEST = 3
    SCRIPT = 4


class PrimitiveFlags(ConfigFlags):
    PRIMITIVE_AMPLITUDE =   0x1
    FREQUENCY =             0x2
    SQUARE_FACTOR =         0x4
    WAVEFORM =              0x8


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


class OutputFlags(ConfigFlags):
    FILEPATH = 0x1
    NORMALISE = 0x2

