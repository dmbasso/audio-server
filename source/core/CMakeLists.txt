cmake_minimum_required(VERSION 2.8.9)

set(CORE_SOURCES
    wav_file.cpp
    wav_file.h
    location.cpp
    location.h
    sound_buffer.cpp
    sound_buffer.h
    core.cpp
    core.h
)

add_subdirectory(generator)
add_subdirectory(processor)
add_subdirectory(output)

add_library(core ${CORE_SOURCES})
target_link_libraries(core generator processor output)
