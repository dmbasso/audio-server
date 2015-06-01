cmake_minimum_required(VERSION 2.8.9)

set(PROCESSOR_SOURCES
    base.h
    base.cpp
    processor.h
    no_operation.cpp
    no_operation.h
    distance_attenuation.cpp
    distance_attenuation.h
)

if(WITH_AAVE)
    set(PROCESSOR_SOURCES
        ${PROCESSOR_SOURCES}
        acousticave.cpp
        acousticave.h
    )
endif(WITH_AAVE)


add_library(processor ${PROCESSOR_SOURCES})

if(WITH_AAVE)
    target_link_libraries(processor ${AAVE_LIBRARY})
endif(WITH_AAVE)
