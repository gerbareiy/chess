cmake_minimum_required(VERSION 3.30)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(GTest CONFIG REQUIRED)

file(GLOB_RECURSE E2E_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.ixx"
)
add_executable(e2e_tests ${E2E_SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${E2E_SOURCES})

target_link_libraries(e2e_tests PRIVATE GTest::gtest_main)
target_include_directories(e2e_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_dependencies(e2e_tests console server)

set_target_properties(e2e_tests PROPERTIES VS_GLOBAL_RunSettingsFilePath "${CMAKE_CURRENT_SOURCE_DIR}/e2e.runsettings")

include(GoogleTest)
gtest_discover_tests(e2e_tests PROPERTIES TIMEOUT 180)
