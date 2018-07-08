cmake_minimum_required(VERSION 3.5)
project(ringbuffer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")


### Files ####
file(GLOB project_sources
        src/ringbuffer.h
        src/ringbuffer.c)
set(SOURCE_FILES ${project_sources})

file(GLOB project_tests test/*.cpp Catch/catch.hpp)
set(TEST_FILES ${project_tests})


### Build executable that runs all tests ###
add_executable(ringbuffer_test ${SOURCE_FILES} ${TEST_FILES} test/main_test.cpp)
