cmake_minimum_required(VERSION 3.23)
project(fm-demo)
set(CMAKE_CXX_STANDARD 17)
add_subdirectory(third_party/wxWidgets)
add_subdirectory(third_party/wxmathplot)
add_subdirectory(third_party/portaudio)

# no dependencies on math only code
add_library(block1 block1.cpp)

add_executable(${CMAKE_PROJECT_NAME}-gui gui.cpp)
target_link_libraries(gui PUBLIC wx::core wx::base block1 wxmathplot)
target_include_directories(gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/wxmathplot)
