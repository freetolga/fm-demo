cmake_minimum_required(VERSION 3.25)
project(wxMathPlot)
set(LIB_VERSION_MAJOR "2")
set(LIB_VERSION_MINOR "0")
set(LIB_VERSION_PATCH "0")
if (MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra)
endif()
# wxMathPlot lib
set(wxmathplot_src mathplot/mathplot.cpp MathPlotConfig/MathPlotConfig.cpp)
add_library(wxmathplot SHARED ${wxmathplot_src})
add_library(wxmathplot_static ${wxmathplot_src})
target_link_libraries(wxmathplot wx::core wx::base wx::adv wx::aui)
target_link_libraries(wxmathplot_static wx::core wx::base wx::adv wx::aui)
include_directories(${CMAKE_SOURCE_DIR}/MathPlotConfig)
include_directories(${CMAKE_SOURCE_DIR}/mathplot)
INSTALL(TARGETS wxmathplot
	RUNTIME DESTINATION bin COMPONENT Applications
	LIBRARY DESTINATION lib COMPONENT Libraries
)
install(FILES mathplot/mathplot.h DESTINATION include/wxmathplot COMPONENT Development)
install(FILES MathPlotConfig/MathPlotConfig.h DESTINATION include/wxmathplot COMPONENT Development)


#CPack

set(CPACK_PACKAGE_DESCRIPTION "wxMathPlot")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "wxMathPlot")
set(CPACK_PACKAGE_VENDOR "wxMathPlot")
set(CPACK_PACKAGE_CONTACT "https://github.com/GitHubLionel/wxMathPlot")
set(CPACK_PACKAGE_VERSION_MAJOR "${LIB_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${LIB_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${LIB_VERSION_PATCH}")
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${LIB_VERSION_MAJOR}.${LIB_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}_${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${LIB_VERSION_MAJOR}.${LIB_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

set(CPACK_COMPONENTS_ALL Libraries Development Applications)
include(CPack)
