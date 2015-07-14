cmake_minimum_required(VERSION 2.8.9)

set(BINDINGS_SOURCES
    cshim.cpp
    cshim.h
)

add_library(cshim SHARED ${BINDINGS_SOURCES})
target_link_libraries(cshim asound core pthread)

