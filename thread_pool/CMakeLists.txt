cmake_minimum_required(VERSION 3.8)
project(thread_pool)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Werror -pthread")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

set(SOURCE_FILES src/main.cpp src/catch.hpp src/tests.cpp src/thread_pool.hpp)
add_executable(thread_pool ${SOURCE_FILES})