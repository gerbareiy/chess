function(collect_sources OUT_VAR)
    file(GLOB_RECURSE COLLECTED_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.ixx"
    )
    set(${OUT_VAR} ${COLLECTED_SOURCES} PARENT_SCOPE)
endfunction()
