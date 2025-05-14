#include <iostream>
#include <string>
#include <ast.h>
#include <ast_builder.h>
#include <nfa.h>
#include <dfa.h>
#include <memory>
#include <my_regex.h>
#include <gtest/gtest.h>

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

	return 0;
}
