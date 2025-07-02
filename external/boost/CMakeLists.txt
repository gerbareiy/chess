include(ExternalProject)

set(project boost)
set(prefix ${CMAKE_CURRENT_BINARY_DIR})

if (MSVC)
    # When MSVC_TOOLSET_VERSION is "141", we want toolset to be "msvc-14.1"
    string(REGEX REPLACE "([0-9]+)([0-9])" "msvc-\\1.\\2" toolset ${MSVC_TOOLSET_VERSION})
else()
    message(FATAL_ERROR "Unsupported compiler. Only Visual Studio is currently supported.")
endif()

math(EXPR ptr_bits "${CMAKE_SIZEOF_VOID_P} * 8")

if (WIN32)
    set(configure_cmd bootstrap.bat)
else()
    set(configure_cmd bootstrap.sh)
endif()

set(build_cmd
    b2
    --with-system
    --with-thread
    --with-atomic
    --with-filesystem
    --with-date_time
    --with-regex
    --with-program_options
    --with-locale
    --with-stacktrace
    toolset=${toolset}
    threading=multi
    link=static
    variant=debug,release
    address-model=${ptr_bits}
    stage
)

set(version 1.88.0)

ExternalProject_Add(
    ${project}
    URL https://archives.boost.io/release/1.88.0/source/boost_1_88_0.zip
    URL_HASH SHA256=8ee21476f1aca1978339f0f4a218b9b8a6746eec83070f32630f97b09c7e91b7
    PREFIX ${prefix}
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ${configure_cmd}
    BUILD_COMMAND ${build_cmd}
    INSTALL_COMMAND ""
    USES_TERMINAL_DOWNLOAD 1
    USES_TERMINAL_CONFIGURE 1
    USES_TERMINAL_BUILD 1
    USES_TERMINAL_INSTALL 1
)

# This variable is used by find_package(Boost)
set(BOOST_DIR "${prefix}/src/${project}/stage/lib/cmake/Boost-${version}" PARENT_SCOPE)
