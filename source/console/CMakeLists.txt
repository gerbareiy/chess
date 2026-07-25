cmake_minimum_required(VERSION 3.30)

collect_sources(CONSOLE_SOURCES)
add_executable(console ${CONSOLE_SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${CONSOLE_SOURCES})

target_include_directories(console PUBLIC ${CMAKE_SOURCE_DIR}/source)
