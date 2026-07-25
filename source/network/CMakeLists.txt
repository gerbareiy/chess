cmake_minimum_required(VERSION 3.30)

collect_sources(NETWORK_SOURCES)
add_library(network ${NETWORK_SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${NETWORK_SOURCES})

target_include_directories(network PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_link_libraries(network PRIVATE cppzmq)
target_link_libraries(network PRIVATE protobuf::libprotobuf)
