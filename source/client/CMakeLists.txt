cmake_minimum_required(VERSION 3.30)

file(GLOB_RECURSE CLIENT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.ixx"
)
add_library(client ${CLIENT_SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${CLIENT_SOURCES})

target_include_directories(client PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_include_directories(client PRIVATE ${Boost_INCLUDE_DIRS})
target_compile_definitions(client PRIVATE _WIN32_WINNT=0x0A00)
target_link_libraries(client PRIVATE Boost::asio Boost::system)
