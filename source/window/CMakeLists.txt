cmake_minimum_required(VERSION 3.30)

collect_sources(WINDOW_SOURCES)
add_executable(window ${WINDOW_SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${WINDOW_SOURCES})

target_include_directories(window PUBLIC ${CMAKE_SOURCE_DIR}/source)
