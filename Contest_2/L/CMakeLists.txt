cmake_minimum_required(VERSION 3.8)
project(L)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY})

set(SOURCE_FILES main.cpp)
add_executable(L ${SOURCE_FILES})