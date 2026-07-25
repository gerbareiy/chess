cmake_minimum_required(VERSION 3.30)

collect_sources(CONSTANTS_SOURCES)
add_library(constants ${CONSTANTS_SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${CONSTANTS_SOURCES})

target_include_directories(constants PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_link_libraries(constants PRIVATE Vulkan::Vulkan)
