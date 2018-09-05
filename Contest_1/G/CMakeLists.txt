cmake_minimum_required(VERSION 3.7)
project(G)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY})

set(SOURCE_FILES main.cpp catch.hpp)
add_executable(G ${SOURCE_FILES})