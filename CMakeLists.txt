cmake_minimum_required(VERSION 3.23)
project(fm-demo)
set(CMAKE_CXX_STANDARD 17)
add_subdirectory(third_party/wxWidgets)
add_subdirectory(third_party/wxMathPlot)
add_subdirectory(third_party/portaudio)

# no dependencies on math only code
add_library(block1 block1.cpp fft.cpp)

if (MSVC)
    add_executable(gui WIN32 gui.cpp)
    set(MY_PATH "PATH=%PATH%" ${CMAKE_BUILD_RPATH}/third_party/wxWidgets/lib/)
    set_target_properties(gui PROPERTIES VS_DEBUGGER_ENVIRONMENT "${MY_PATH}")
else ()
    add_executable(gui gui.cpp)
endif ()
target_link_libraries(gui PUBLIC wx::core wx::base block1 wxmathplot)
target_include_directories(gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/wxMathPlot/mathplot)
