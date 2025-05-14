cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 23)

add_library(ast_builder ast_builder.cpp)

target_include_directories(ast_builder PUBLIC .)

target_link_libraries(ast_builder ast)
