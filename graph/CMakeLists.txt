cmake_minimum_required(VERSION 3.7)
project(graph)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wshadow -pedantic -Wc++1z-extensions")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
set(SOURCE_FILES src/main.cpp src/graph.h src/iterator.hpp src/catch.hpp src/filtered_graph.h src/shortest_path.h src/shared_graph.h)
add_executable(graph ${SOURCE_FILES})