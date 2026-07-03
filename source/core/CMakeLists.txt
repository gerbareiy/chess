cmake_minimum_required(VERSION 3.30)

file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.ixx"
)
add_library(core ${CORE_SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${CORE_SOURCES})

target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_include_directories(core PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(core PRIVATE Boost::signals2 Boost::json)
