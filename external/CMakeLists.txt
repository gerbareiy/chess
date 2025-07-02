cmake_minimum_required(VERSION 3.30)

project("chess dependencies")

add_subdirectory(build/boost)

configure_file("chess-deps.cmake.in" "${CMAKE_BINARY_DIR}/chess-deps.cmake")
