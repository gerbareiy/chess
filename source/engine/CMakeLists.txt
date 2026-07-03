cmake_minimum_required(VERSION 3.30)

file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.ixx"
)
add_library(engine ${ENGINE_SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${ENGINE_SOURCES})

target_include_directories(engine PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_link_libraries(engine PRIVATE glfw)
target_link_libraries(engine PRIVATE glm::glm)
target_link_libraries(engine PRIVATE magic_enum::magic_enum)
target_link_libraries(engine PRIVATE Vulkan::Vulkan)
