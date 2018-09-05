cmake_minimum_required(VERSION 3.7)
project(H)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY})
set(SOURCE_FILES main.cpp cmake-build-debug/link_cut.h)
add_executable(H ${SOURCE_FILES})