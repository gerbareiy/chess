cmake_minimum_required(VERSION 3.30)

file(GLOB_RECURSE CONSOLE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.ixx"
)
add_executable(console ${CONSOLE_SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${CONSOLE_SOURCES})

target_include_directories(console PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_include_directories(console PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(console PRIVATE Boost::signals2)
