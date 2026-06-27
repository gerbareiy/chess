cmake_minimum_required(VERSION 3.30)

file(GLOB_RECURSE WINDOW_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.ixx"
)
add_executable(window ${WINDOW_SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${WINDOW_SOURCES})

target_include_directories(window PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_link_libraries(window PRIVATE glfw)
target_link_libraries(window PRIVATE  glm::glm)
