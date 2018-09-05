cmake_minimum_required(VERSION 3.8)
project(J)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY})
add_executable(J ${SOURCE_FILES})