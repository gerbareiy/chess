# general info
cmake_minimum_required(VERSION 3.30)

# common options
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(console)
add_subdirectory(core)

target_link_libraries(console PRIVATE core)

target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_include_directories(console PUBLIC ${CMAKE_SOURCE_DIR}/source)
