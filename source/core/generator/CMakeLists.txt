cmake_minimum_required(VERSION 2.8.9)

set(GENERATOR_SOURCES
    base.cpp
    generator.h
    script.cpp
    script.h
    wave.cpp
    wave.h
    test.cpp
    test.h
    base.h
    primitive.cpp
    primitive.h
)

add_library(generator ${GENERATOR_SOURCES})
target_link_libraries(generator core)
