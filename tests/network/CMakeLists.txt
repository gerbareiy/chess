cmake_minimum_required(VERSION 3.30)

file(GLOB_RECURSE TEST_NETWORK_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
add_executable(network_tests ${TEST_NETWORK_SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${TEST_NETWORK_SOURCES})

target_link_libraries(network_tests PRIVATE network core GTest::gtest_main)
target_include_directories(network_tests PRIVATE ${CMAKE_SOURCE_DIR}/source)

set_target_properties(network_tests PROPERTIES VS_GLOBAL_RunSettingsFilePath "${CMAKE_SOURCE_DIR}/chess.runsettings")

include(GoogleTest)
gtest_discover_tests(network_tests)
