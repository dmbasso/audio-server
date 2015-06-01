cmake_minimum_required(VERSION 2.8.9)
project(audio-server)

FIND_LIBRARY(AAVE_LIBRARY libaave.a "../libaave/libaave" "../libaave")
FIND_PATH(AAVE_INCLUDE_DIRS aave.h "../libaave/libaave" "../libaave")

FIND_PACKAGE(ALSA REQUIRED)  # "libasound2-dev" on Debian based distros

OPTION(WITH_AAVE "Link against libaave" ON)

IF(AAVE_LIBRARY)
    ADD_DEFINITIONS(-DWITH_AAVE)
    INCLUDE_DIRECTORIES(${AAVE_INCLUDE_DIRS})
ELSE()
    SET(WITH_AAVE OFF)
ENDIF(AAVE_LIBRARY)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")

add_subdirectory(source)
