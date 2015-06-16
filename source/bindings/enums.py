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
    NOISE = 5


class PrimitiveFlags(ConfigFlags):
    PRIMITIVE_AMPLITUDE =   0x1
    FREQUENCY =             0x2
    SQUARE_FACTOR =         0x4
    WAVEFORM =              0x8


class WaveformType:
    SINE = 1
    SQUARE = 2
    SAWTOOTH = 3


class WaveFlags(ConfigFlags):
    WAVE_INDEX =        0x1
    FREQUENCY_RATIO =   0x2
    POSITION =          0x4
    PLAYBACK_COMMAND =  0x8


class KeyframeFlags(WaveFlags):
    LOCATION =  0x10000
    END =       0x20000


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


class ScriptCommand(PlaybackCommand):
    ADD_KEYFRAMES = 100
    RESET_KEYFRAMES = 101


class ScriptFlags(ConfigFlags):
    COMMAND = 0x1


# Processors


class AcousticaveFlags(ConfigFlags):
    AAVE_LISTENER_POSITION =          0x1
    MODEL_FILEPATH =              0x10000
    GAIN =                        0x20000
    REFLECTIONS =                 0X40000
    HRTF =                        0x80000
    REVERB_ACTIVE =              0x100000
    RT60 =                       0x200000
    AREA =                       0x400000
    VOLUME =                     0x800000
    AAVE_LISTENER_ORIENTATION = 0x1000000


class ProcessorType:
    NO_OPERATION = 1
    ACOUSTICAVE = 2
    DISTANCE_ATTENUATION = 3


class AcousticaveHRTF:
    MIT = 1
    CIPIC = 2
    LISTEN = 3
    TUB = 4
    IDENTITY = 5


# Output

class OutputType:
    FILE = 1
    ALSA = 2
    MEMORY = 3


class OutputFlags(ConfigFlags):
    FILEPATH = 0x1
    NORMALISE = 0x2
