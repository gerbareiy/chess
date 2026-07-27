cmake_minimum_required(VERSION 3.30)

add_library(client)
add_target_sources(client)

target_include_directories(client PUBLIC ${CMAKE_SOURCE_DIR}/source)
