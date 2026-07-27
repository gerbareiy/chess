cmake_minimum_required(VERSION 3.30)

add_executable(console)
add_target_sources(console)

target_include_directories(console PUBLIC ${CMAKE_SOURCE_DIR}/source)
