cmake_minimum_required(VERSION 3.30)

file(GLOB_RECURSE TEST_UTILS_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
add_executable(utils_tests ${TEST_UTILS_SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${TEST_UTILS_SOURCES})

target_link_libraries(utils_tests PRIVATE utils GTest::gtest_main)
target_include_directories(utils_tests PRIVATE ${CMAKE_SOURCE_DIR}/source)

set_target_properties(utils_tests PROPERTIES VS_GLOBAL_RunSettingsFilePath "${CMAKE_SOURCE_DIR}/chess.runsettings")

include(GoogleTest)
gtest_discover_tests(utils_tests)
