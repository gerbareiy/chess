cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE CORE_SOURCES *.cpp *.ixx *.h)
add_library(core ${CORE_SOURCES})
