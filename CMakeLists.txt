cmake_minimum_required(VERSION 3.24)
project(Wordlist)
set(CMAKE_CXX_STANDARD 14)

include_directories(src)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src)

add_library(core SHARED
        ${INCLUDE_DIR}/core.h ${INCLUDE_DIR}/core.cpp)

link_directories(${LIBRARY_OUTPUT_PATH})
target_link_libraries(core)

#add_executable(WordList
#        src/error.h src/utils.h src/utils.cpp src/main.cpp)

#add_executable(WordList
#        src/core.cpp src/core.h)