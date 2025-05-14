cmake_minimum_required(VERSION 3.27)
project(lab_2)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(ast)
add_subdirectory(ast_builder)
add_subdirectory(nfa)
add_subdirectory(dfa)
add_subdirectory(my_regex)

add_executable(main main.cpp tests/tests.cpp)
target_link_libraries(main gtest gmock pthread ast ast_builder nfa dfa my_regex)
