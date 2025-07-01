cmake_minimum_required(VERSION 3.20)

add_subdirectory(console)
add_subdirectory(core)

target_link_libraries(console PRIVATE core)

target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/source/core)
target_include_directories(console PUBLIC ${CMAKE_SOURCE_DIR}/source/console)
target_include_directories(console PUBLIC ${CMAKE_SOURCE_DIR}/source/core)
