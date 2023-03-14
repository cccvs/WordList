cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 14)

add_executable(test
        main.cpp
        test.cpp
        test.h
        ${PROJECT_SOURCE_DIR}/src/core.cpp
        ${PROJECT_SOURCE_DIR}/src/core.h)

include_directories(${PROJECT_SOURCE_DIR}/googletest/include)
link_directories(${PROJECT_SOURCE_DIR}/googletest)

target_link_libraries(test
        gtest)
