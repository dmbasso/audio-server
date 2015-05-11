cmake_minimum_required(VERSION 2.8.9)

set(OUTPUT_SOURCES
    memory.cpp
    memory.h
    alsa.cpp
    alsa.h
    file.cpp
    file.h
    base.h
    output.h
)

add_library(output ${OUTPUT_SOURCES})

target_link_libraries(output asound)