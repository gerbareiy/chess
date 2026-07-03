# general info
cmake_minimum_required(VERSION 3.30)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "Vcpkg toolchain file")
endif()

# common options
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Boost REQUIRED COMPONENTS asio json signals2 system)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

add_subdirectory(client)
add_subdirectory(console)
add_subdirectory(core)
add_subdirectory(engine)
add_subdirectory(network)
add_subdirectory(proto)
add_subdirectory(server)
add_subdirectory(utils)
add_subdirectory(window)

target_link_libraries(core PRIVATE utils)

target_link_libraries(network PUBLIC proto core)

target_link_libraries(client PUBLIC core network utils)

target_link_libraries(server PRIVATE core network)

target_link_libraries(console PRIVATE client core utils)

target_link_libraries(engine PRIVATE utils)

target_link_libraries(window PRIVATE engine)
