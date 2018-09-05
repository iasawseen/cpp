cmake_minimum_required(VERSION 3.5)
project(I)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp treap.hpp segment.hpp)
add_executable(I ${SOURCE_FILES})