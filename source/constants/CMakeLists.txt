cmake_minimum_required(VERSION 3.30)

file(GLOB_RECURSE CONSTANTS_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.ixx"
)
add_library(constants ${CONSTANTS_SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${CONSTANTS_SOURCES})

target_include_directories(constants PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_link_libraries(constants PRIVATE Vulkan::Vulkan)
