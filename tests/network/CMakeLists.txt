cmake_minimum_required(VERSION 3.30)

add_executable(network_tests)
add_target_sources(network_tests)

target_link_libraries(network_tests PRIVATE network core GTest::gtest_main)
target_include_directories(network_tests PRIVATE ${CMAKE_SOURCE_DIR}/source)

set_target_properties(network_tests PROPERTIES VS_GLOBAL_RunSettingsFilePath "${CMAKE_SOURCE_DIR}/chess.runsettings")

include(GoogleTest)
gtest_discover_tests(network_tests)
