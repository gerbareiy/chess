cmake_minimum_required(VERSION 3.30)

add_library(core)
add_target_sources(core)

target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_include_directories(core PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(core PRIVATE Boost::json)
