cmake_minimum_required(VERSION 3.30)

file(GLOB_RECURSE TEST_CORE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
add_executable(core_tests ${TEST_CORE_SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${TEST_CORE_SOURCES})

target_link_libraries(core_tests PRIVATE core GTest::gtest_main)
target_include_directories(core_tests PRIVATE ${CMAKE_SOURCE_DIR}/source)

set_target_properties(core_tests PROPERTIES VS_GLOBAL_RunSettingsFilePath "${CMAKE_SOURCE_DIR}/chess.runsettings")

include(GoogleTest)
gtest_discover_tests(core_tests)
