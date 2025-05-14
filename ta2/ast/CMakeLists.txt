cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 23)

add_library(ast STATIC ast.cpp)

target_include_directories(ast PUBLIC .)
