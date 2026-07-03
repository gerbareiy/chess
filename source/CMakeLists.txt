# general info
cmake_minimum_required(VERSION 3.30)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "Vcpkg toolchain file")
endif()

# common options
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Boost REQUIRED COMPONENTS json signals2)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

add_subdirectory(console)
add_subdirectory(engine)
add_subdirectory(server)
add_subdirectory(utils)
add_subdirectory(window)

target_link_libraries(console PRIVATE server)
target_link_libraries(console PRIVATE utils)

target_link_libraries(engine PRIVATE utils)

target_link_libraries(server PRIVATE utils)

target_link_libraries(window PRIVATE engine)

