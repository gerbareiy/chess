cmake_minimum_required(VERSION 3.30)

add_executable(server)
add_target_sources(server)

target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/source)
