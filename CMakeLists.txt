cmake_minimum_required(VERSION 3.23)

find_package(wxWidgets REQUIRED)
add_subdirectory(wxmathplot)
find_package(portaudio19)
project(eee302-project)
add_library(block1 block1.cpp
    block1.h)
set_property(TARGET block1 PROPERTY CXX_STANDARD 26)
add_executable(gui gui.cpp
    misc_utils.cpp)
set_property(TARGET gui PROPERTY CXX_STANDARD 26)
target_link_libraries(gui PUBLIC wxWidgets::wxWidgets block1 wxmathplot)
target_include_directories(gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/wxmathplot)
