# general info
cmake_minimum_required(VERSION 3.30)
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "Vcpkg toolchain file")
endif()

# common options
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Boost REQUIRED COMPONENTS json signals2)

add_subdirectory(console)
add_subdirectory(core)

target_link_libraries(console PRIVATE core)

# core options
target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_include_directories(core PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(core PRIVATE Boost::signals2 Boost::json)

# console options
target_include_directories(console PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_include_directories(console PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(console PRIVATE Boost::signals2)
