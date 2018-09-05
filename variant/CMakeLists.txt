cmake_minimum_required(VERSION 3.7)
project(variant)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

set(SOURCE_FILES src/test.cpp src/catch.hpp src/variant.h)
add_executable(variant ${SOURCE_FILES})