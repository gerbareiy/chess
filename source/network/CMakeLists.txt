cmake_minimum_required(VERSION 3.30)

add_library(network)
add_target_sources(network)

target_include_directories(network PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_link_libraries(network PRIVATE cppzmq)
target_link_libraries(network PRIVATE protobuf::libprotobuf)
