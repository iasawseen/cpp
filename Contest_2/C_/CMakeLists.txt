cmake_minimum_required(VERSION 3.7)
project(C_)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY})
set(SOURCE_FILES main.cpp)
add_executable(C_ ${SOURCE_FILES})