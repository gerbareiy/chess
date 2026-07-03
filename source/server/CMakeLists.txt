cmake_minimum_required(VERSION 3.30)

file(GLOB_RECURSE SERVER_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.ixx"
)
add_executable(server ${SERVER_SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SERVER_SOURCES})

target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/source)
target_include_directories(server PRIVATE ${Boost_INCLUDE_DIRS})
target_compile_definitions(server PRIVATE _WIN32_WINNT=0x0A00)
target_link_libraries(server PRIVATE Boost::asio Boost::system)
