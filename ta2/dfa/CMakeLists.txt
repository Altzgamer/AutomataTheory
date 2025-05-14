cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 23)

add_library(dfa dfa.cpp)

target_include_directories(dfa PUBLIC .)

target_link_libraries(dfa nfa ast ast_builder)
