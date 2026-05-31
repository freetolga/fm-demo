cmake_minimum_required(VERSION 3.23)

add_subdirectory(wxwidgets)
add_subdirectory(wxmathplot)
add_subdirectory(portaudio)
project(eee302-project)
add_library(block1 block1.cpp
    block1.h)
set_property(TARGET block1 PROPERTY CXX_STANDARD 26)
add_executable(gui gui.cpp)
set_property(TARGET gui PROPERTY CXX_STANDARD 26)
target_link_libraries(gui PUBLIC wx::net wx::core wx::base block1 wxmathplot)
target_include_directories(gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/wxmathplot)