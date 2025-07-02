cmake_minimum_required(VERSION 3.30)

include(FetchContent)

FetchContent_Declare(
    Boost
    URL https://archives.boost.io/release/1.88.0/source/boost_1_88_0.zip
    URL_HASH SHA256=8ee21476f1aca1978339f0f4a218b9b8a6746eec83070f32630f97b09c7e91b7
)

FetchContent_MakeAvailable(Boost)

set(BOOST_ROOT ${boost_SOURCE_DIR})
set(BOOST_INCLUDEDIR ${BOOST_ROOT})

# external options
find_package(Boost 1.88.0 REQUIRED COMPONENTS filesystem system)
