cmake_minimum_required(VERSION 3.23)

add_subdirectory(wxwidgets)
add_subdirectory(matplotplusplus)
add_subdirectory(portaudio)
project(eee302-project)
add_library(block1 block1.cpp
    block1.h)
set_property(TARGET block1 PROPERTY CXX_STANDARD 26)
add_executable(gui gui.cpp)
set_property(TARGET gui PROPERTY CXX_STANDARD 26)
target_link_libraries(gui wx::net wx::core wx::base matplot block1)