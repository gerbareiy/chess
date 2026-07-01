cmake_minimum_required(VERSION 3.30)

file(GLOB_RECURSE TEST_SERVER_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
add_executable(server_tests ${TEST_SERVER_SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${TEST_SERVER_SOURCES})

target_link_libraries(server_tests PRIVATE server GTest::gtest_main)
target_include_directories(server_tests PRIVATE ${CMAKE_SOURCE_DIR}/source)

set_target_properties(server_tests PROPERTIES VS_GLOBAL_RunSettingsFilePath "${CMAKE_SOURCE_DIR}/chess.runsettings")

include(GoogleTest)
gtest_discover_tests(server_tests)
