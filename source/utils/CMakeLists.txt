cmake_minimum_required(VERSION 3.30)

add_library(utils)
add_target_sources(utils)

target_include_directories(utils PUBLIC ${CMAKE_SOURCE_DIR}/source)
