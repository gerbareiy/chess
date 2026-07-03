cmake_minimum_required(VERSION 3.30)

file(GLOB PROTO_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.proto")

add_library(proto STATIC ${PROTO_FILES})

target_link_libraries(proto PUBLIC protobuf::libprotobuf)

target_include_directories(proto PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate(
    TARGET proto
    LANGUAGE cpp
    IMPORT_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
    PROTOC_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}"
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${PROTO_FILES})
