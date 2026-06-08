# overhauled makefile

cmake_minimum_required(VERSION 3.23)
add_library(wxmathplot mathplot.cpp mathplot.h)
target_link_libraries(wxmathplot wx::base wx::core)
