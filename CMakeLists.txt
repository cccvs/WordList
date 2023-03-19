cmake_minimum_required(VERSION 3.24)

project(Wordlist)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(src)
add_subdirectory(gui)
add_subdirectory(test)
add_subdirectory(combine)

set(GOOGLETEST_VERSION 1.13.0)
add_subdirectory(googletest)
