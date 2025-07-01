cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE CONSOLE_SOURCES *.cpp *.ixx *.h)
add_executable(console ${CONSOLE_SOURCES})
