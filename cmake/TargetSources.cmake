function(add_target_sources TARGET)
    file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    )
    file(GLOB_RECURSE MODULES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.ixx"
    )

    if(SOURCES)
        target_sources(${TARGET} PRIVATE ${SOURCES})
    endif()

    if(MODULES)
        get_target_property(TARGET_TYPE ${TARGET} TYPE)
        if(TARGET_TYPE STREQUAL "EXECUTABLE")
            set(MODULES_SCOPE PRIVATE)
        else()
            set(MODULES_SCOPE PUBLIC)
        endif()
        target_sources(${TARGET} ${MODULES_SCOPE} FILE_SET CXX_MODULES FILES ${MODULES})
    endif()
    source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES} ${MODULES})
endfunction()
