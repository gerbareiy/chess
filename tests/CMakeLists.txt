cmake_minimum_required(VERSION 3.30)

find_package(GTest CONFIG REQUIRED)

add_subdirectory(core)
add_subdirectory(network)

target_link_libraries(network_tests PRIVATE proto)
