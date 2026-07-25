cmake_minimum_required(VERSION 3.30)

collect_sources(CORE_SOURCES)
add_library(core ${CORE_SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${CORE_SOURCES})

target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_include_directories(core PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(core PRIVATE Boost::json)
