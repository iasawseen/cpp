cmake_minimum_required(VERSION 3.7)
project(M)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY})

set(SOURCE_FILES main.cpp catch.hpp garbage.hpp)

add_executable(M ${SOURCE_FILES})