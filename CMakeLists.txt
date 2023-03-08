cmake_minimum_required(VERSION 3.24)
project(WordList)

set(CMAKE_CXX_STANDARD 14)

add_executable(WordList
        src/utils.cpp src/core.h
        src/test.cpp src/main.cpp)

include_directories(src)
