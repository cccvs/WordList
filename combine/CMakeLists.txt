cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_EXE_LINKER_FLAGS "-static")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/combine/their_core)

include_directories(${PROJECT_SOURCE_DIR}/googletest/include)
link_directories(${PROJECT_SOURCE_DIR}/googletest)

add_executable(test_combine
        ${PROJECT_SOURCE_DIR}/test/main.cpp
        ${PROJECT_SOURCE_DIR}/test/core_test.cpp
        ${PROJECT_SOURCE_DIR}/test/test.h
        ${PROJECT_SOURCE_DIR}/src/core.h)

target_link_libraries(test_combine
        gtest
        ${PROJECT_SOURCE_DIR}/combine/their_core/core.dll)