cmake_minimum_required(VERSION 2.8.9)
project(audio-server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    source/generator/generator.cpp
    source/generator/generator.h
    source/generator/generator_script.cpp
    source/generator/generator_script.h
    source/generator/generator_wave.cpp
    source/generator/generator_wave.h
    source/generator/generator_test.cpp
    source/generator/generator_test.h
    source/generator/generator_primitive.cpp
    source/generator/generator_primitive.h
    source/processor/processor.cpp
    source/processor/processor.h
    source/processor/processor_nooperation.cpp
    source/processor/processor_nooperation.h
    source/processor/processor_acousticave.cpp
    source/processor/processor_acousticave.h
    source/processor/processor_distance_attenuation.cpp
    source/processor/processor_distance_attenuation.h
    source/wav_file.cpp
    source/wav_file.h
    source/location.cpp
    source/location.h
    source/sound_buffer.cpp
    source/sound_buffer.h
    source/core.cpp
    source/core.h
    source/main.cpp
    source/output.cpp
    source/output.h)
add_executable(audio-server ${SOURCE_FILES})