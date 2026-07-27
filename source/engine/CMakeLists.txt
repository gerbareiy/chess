cmake_minimum_required(VERSION 3.30)

add_library(engine)
add_target_sources(engine)

target_include_directories(engine PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_link_libraries(engine PRIVATE glfw)
target_link_libraries(engine PRIVATE glm::glm)
target_link_libraries(engine PRIVATE magic_enum::magic_enum)
target_link_libraries(engine PRIVATE Vulkan::Vulkan)
