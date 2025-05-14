cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 23)

add_library(nfa nfa.cpp)

target_include_directories(nfa PUBLIC .)

target_link_libraries(nfa ast ast_builder)
