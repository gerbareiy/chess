# general options
cmake_minimum_required(VERSION 3.30)

# cpp options
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# solution options
project(chess)

include(${CMAKE_CURRENT_SOURCE_DIR}/external/boost/CMakeLists.txt)

add_subdirectory(source)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT console)
