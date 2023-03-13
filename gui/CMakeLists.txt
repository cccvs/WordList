cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#set(CMAKE_EXE_LINKER_FLAGS "-static")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/guibin)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(gui WIN32
        main.cpp
        view.cpp
        ../src/utils.cpp)

target_link_libraries(gui
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )
