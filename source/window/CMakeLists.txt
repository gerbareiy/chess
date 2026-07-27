cmake_minimum_required(VERSION 3.30)

add_executable(window)
add_target_sources(window)

target_include_directories(window PUBLIC ${CMAKE_SOURCE_DIR}/source)
