cmake_minimum_required(VERSION 3.30)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(GTest CONFIG REQUIRED)

add_executable(e2e_tests)
add_target_sources(e2e_tests)

target_link_libraries(e2e_tests PRIVATE GTest::gtest_main)
target_link_libraries(e2e_tests PRIVATE utils)
target_include_directories(e2e_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_dependencies(e2e_tests console server)

set_target_properties(e2e_tests PROPERTIES VS_GLOBAL_RunSettingsFilePath "${CMAKE_CURRENT_SOURCE_DIR}/e2e.runsettings")

include(GoogleTest)
gtest_discover_tests(e2e_tests PROPERTIES TIMEOUT 180)
