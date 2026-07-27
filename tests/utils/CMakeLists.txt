cmake_minimum_required(VERSION 3.30)

add_executable(utils_tests)
add_target_sources(utils_tests)

target_link_libraries(utils_tests PRIVATE utils GTest::gtest_main)
target_include_directories(utils_tests PRIVATE ${CMAKE_SOURCE_DIR}/source)

set_target_properties(utils_tests PROPERTIES VS_GLOBAL_RunSettingsFilePath "${CMAKE_SOURCE_DIR}/chess.runsettings")

include(GoogleTest)
gtest_discover_tests(utils_tests)
