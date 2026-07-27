cmake_minimum_required(VERSION 3.30)

add_library(constants)
add_target_sources(constants)

target_include_directories(constants PUBLIC ${CMAKE_SOURCE_DIR}/source)
