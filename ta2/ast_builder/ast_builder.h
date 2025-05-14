#pragma once

#include "../ast/ast.h"
#include <string>
#include <vector>

namespace Ast
{
	class Ast_builder
	{
		public:
			static AST BuildAst(const std::string& expr);

		private:
			static void FormAstLists(std::vector <AST>& asts, const std::string& expr, size_t& brackets);
			static std::pair <size_t, size_t> FindClosestBrackets(std::vector <AST>& asts);
			static void CreateAstBetweenBrackets(std::pair <size_t, size_t>& closest_brackets, std::vector <AST>& asts);
			static void ConcatinateAsts(std::vector <AST>& asts);
	};
}
