# general options
cmake_minimum_required(VERSION 3.30)

# cpp options
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# solution options
project(chess)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CollectSources)

add_subdirectory(source)

enable_testing()
add_subdirectory(tests)
add_subdirectory(e2e)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT console)
