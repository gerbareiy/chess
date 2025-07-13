cmake_minimum_required(VERSION 3.30)

include(FetchContent)

set(FETCHCONTENT_QUIET OFF)

function(add_boost_part part_name)
    FetchContent_Declare(
        ${part_name}
        GIT_REPOSITORY https://github.com/boostorg/${part_name}.git
        GIT_PROGRESS ON
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL ON
    )

    FetchContent_MakeAvailable(${part_name})
endfunction()

add_boost_part("type_index")
add_boost_part("integer")
add_boost_part("functional")
add_boost_part("typeof")
add_boost_part("variant")
add_boost_part("mp11")
add_boost_part("tuple")
add_boost_part("smart_ptr")
add_boost_part("describe")
add_boost_part("io")
add_boost_part("function_types")
add_boost_part("parameter")
add_boost_part("container_hash")
add_boost_part("utility")
add_boost_part("optional")
add_boost_part("type_traits")
add_boost_part("predef")
add_boost_part("fusion")
add_boost_part("preprocessor")
add_boost_part("detail")
add_boost_part("mpl")
add_boost_part("concept_check")
add_boost_part("move")
add_boost_part("throw_exception")
add_boost_part("iterator")
add_boost_part("static_assert")
add_boost_part("function")
add_boost_part("core")
add_boost_part("config")
add_boost_part("bind")
add_boost_part("assert")
add_boost_part("signals2")
