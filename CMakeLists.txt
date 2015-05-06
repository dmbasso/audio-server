cmake_minimum_required(VERSION 2.8.9)
project(audio-server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

FIND_LIBRARY(AAVE_LIBRARY libaave.a "../libaave")

set(SOURCE_FILES
    source/temp.cpp
    source/temp.h
    source/output/memory.cpp
    source/output/memory.h
    source/output/alsa.cpp
    source/output/alsa.h
    source/output/file.cpp
    source/output/file.h
    source/output/output.h
    source/generator/generator.cpp
    source/generator/generator.h
    source/generator/script.cpp
    source/generator/script.h
    source/generator/wave.cpp
    source/generator/wave.h
    source/generator/test.cpp
    source/generator/test.h
    source/generator/primitive.cpp
    source/generator/primitive.h
    source/processor/processor.cpp
    source/processor/processor.h
    source/processor/no_operation.cpp
    source/processor/no_operation.h
    source/processor/acousticave.cpp
    source/processor/acousticave.h
    source/processor/distance_attenuation.cpp
    source/processor/distance_attenuation.h
    source/wav_file.cpp
    source/wav_file.h
    source/location.cpp
    source/location.h
    source/sound_buffer.cpp
    source/sound_buffer.h
    source/core.cpp
    source/core.h
    source/main.cpp)
add_executable(audio-server ${SOURCE_FILES})

target_link_libraries(audio-server asound ${AAVE_LIBRARY})