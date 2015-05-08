cmake_minimum_required(VERSION 2.8.9)
project(audio-server)

FIND_LIBRARY(AAVE_LIBRARY libaave.a "../libaave/libaave")
FIND_PATH(AAVE_INCLUDE_DIRS aave.h "../libaave/libaave")

INCLUDE_DIRECTORIES(${AAVE_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(source)

