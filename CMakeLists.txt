cmake_minimum_required(VERSION 2.8.9)
project(audio-server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    source/wav_file.cpp
    source/wav_file.h
    source/location.cpp
    source/location.h
    source/sound_buffer.cpp
    source/sound_buffer.h
    source/core.cpp
    source/core.h
    source/generator.cpp
    source/generator.h
    source/main.cpp
    source/output.cpp
    source/output.h
    source/processor.cpp
    source/processor.h)

add_executable(audio-server ${SOURCE_FILES})