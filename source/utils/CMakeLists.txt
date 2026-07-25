cmake_minimum_required(VERSION 3.30)

collect_sources(UTILS_SOURCES)
add_library(utils ${UTILS_SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${UTILS_SOURCES})

target_include_directories(utils PUBLIC ${CMAKE_SOURCE_DIR}/source)
