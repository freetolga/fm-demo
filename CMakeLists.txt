cmake_minimum_required(VERSION 3.23)

add_subdirectory(wxwidgets)
add_subdirectory(matplotplusplus)
project(eee302-project)
add_executable(eee302-project main.cpp)
target_link_libraries(eee302-project wx::net wx::core wx::base)