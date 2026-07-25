cmake_minimum_required(VERSION 3.30)

collect_sources(CLIENT_SOURCES)
add_library(client ${CLIENT_SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${CLIENT_SOURCES})

target_include_directories(client PUBLIC ${CMAKE_SOURCE_DIR}/source)
