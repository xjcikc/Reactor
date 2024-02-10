cmake_minimum_required(VERSION 3.20)

project(reactor)

aux_source_directory(src SRC_LIST)

add_executable(server ${SRC_LIST})

target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)