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
add_subdirectory(constants)
add_subdirectory(core)
add_subdirectory(engine)
add_subdirectory(network)
add_subdirectory(proto)
add_subdirectory(server)
add_subdirectory(utils)
add_subdirectory(window)

target_link_libraries(client PRIVATE core)
target_link_libraries(client PRIVATE network)
target_link_libraries(client PRIVATE proto)
target_link_libraries(client PRIVATE utils)

target_link_libraries(console PRIVATE client)
target_link_libraries(console PRIVATE constants)
target_link_libraries(console PRIVATE core)
target_link_libraries(console PRIVATE utils)

target_link_libraries(core PRIVATE constants)
target_link_libraries(core PRIVATE utils)

target_link_libraries(engine PRIVATE utils)

target_link_libraries(network PRIVATE core)
target_link_libraries(network PRIVATE proto)

target_link_libraries(server PRIVATE core)
target_link_libraries(server PRIVATE network)

target_link_libraries(window PRIVATE constants)
target_link_libraries(window PRIVATE engine)
