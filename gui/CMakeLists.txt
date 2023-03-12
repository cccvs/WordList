cmake_minimum_required(VERSION 3.24)
project(word_list_gui)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_EXE_LINKER_FLAGS "-static")

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(word_list_gui WIN32
        main.cpp
        view.cpp
        ../src/utils.cpp)

include_directories(gui)


target_link_libraries(word_list_gui
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )
