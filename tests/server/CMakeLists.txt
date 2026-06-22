cmake_minimum_required(VERSION 3.30)

file(GLOB_RECURSE TEST_SERVER_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
add_executable(unit_tests ${TEST_SERVER_SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${TEST_SERVER_SOURCES})

target_link_libraries(unit_tests PRIVATE server GTest::gtest_main)
target_include_directories(unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/source)

include(GoogleTest)
gtest_discover_tests(unit_tests)
