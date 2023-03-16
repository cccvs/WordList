cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 14)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_SHARED_LIBRARY_PREFIX "")

set(CMAKE_SHARED_LINKER_FLAGS "-static")
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_library(core SHARED
        core.cpp
        core.h)

add_executable(Wordlist
        main.cpp
        cli.cpp
        cli.h
        utils.cpp
        utils.h)

target_link_libraries(Wordlist core)
