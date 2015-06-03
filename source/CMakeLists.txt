cmake_minimum_required(VERSION 2.8.9)

add_subdirectory(core)

set(MAIN_SOURCES
    main.cpp
    temp.cpp
    temp.h
)

set(RUNTIME_OUTPUT_DIRECTORY "../")

add_executable(audio-server ${MAIN_SOURCES})

target_link_libraries(audio-server core)

add_subdirectory(bindings)

add_custom_target(
    tests py.test -xv tests
    DEPENDS cshim
)
