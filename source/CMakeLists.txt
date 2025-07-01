cmake_minimum_required(VERSION 3.20)

add_subdirectory(console)
add_subdirectory(core)

target_link_libraries(console PRIVATE core)