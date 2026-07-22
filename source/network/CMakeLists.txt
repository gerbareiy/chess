cmake_minimum_required(VERSION 3.30)

find_package(cppzmq CONFIG REQUIRED)

file(GLOB_RECURSE NETWORK_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.ixx"
)
add_library(network ${NETWORK_SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${NETWORK_SOURCES})

target_include_directories(network PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_link_libraries(network PRIVATE cppzmq)
