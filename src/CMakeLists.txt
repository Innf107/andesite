cmake_minimum_required(VERSION 3.20)

project("Andesite")

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
add_compile_options(-Wall -Wextra -std=c++20)

set(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_library(parser "Parser.cpp")
add_library(runtime "Runtime.cpp")
add_executable(andesite "Main.cpp")
target_link_libraries(andesite parser runtime)
