cmake_minimum_required(VERSION 3.30)

collect_sources(SERVER_SOURCES)
add_executable(server ${SERVER_SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SERVER_SOURCES})

target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/source)
