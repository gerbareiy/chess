cmake_minimum_required(VERSION 3.30)

file(GLOB_RECURSE CLIENT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.ixx"
)
add_library(client ${CLIENT_SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${CLIENT_SOURCES})

target_include_directories(client PUBLIC ${CMAKE_SOURCE_DIR}/source)
