cmake_minimum_required(VERSION 2.8.9)

set(PROCESSOR_SOURCES
    base.h
    base.cpp
    processor.h
    no_operation.cpp
    no_operation.h
    acousticave.cpp
    acousticave.h
    distance_attenuation.cpp
    distance_attenuation.h
)

add_library(processor ${PROCESSOR_SOURCES})
target_link_libraries(processor ${AAVE_LIBRARY})
