# -*- coding:utf-8 -*-

from ctypes import (
    Structure, c_byte, c_float, c_int, c_uint,
    c_uint64, c_double, c_ushort
)


# Generators

class GeneratorConfigData(Structure):
    _fields_ = [
        ('flags', c_uint64),
    ]


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


class PrimitiveConfigData(Structure): #GeneratorConfigData):
    _fields_ = [
        ('flags', c_uint64),
        ('amplitude', c_int),
        ('frequency', c_ushort),
        ('frequency', c_ushort),
        ('waveform', c_int),
    ]

class TestConfigData(PrimitiveConfigData):
    _fields_ = [
        ('transitionPeriod', c_uint),
        ('frequencyScaleFactor', c_float),
        ('distance', c_double),
        ('angleStep', c_double),
        ('initialAngle', c_double),
    ]


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

