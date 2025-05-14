#include "ast_builder.h"
#include <stack>

namespace Ast
{

	AST Ast_builder::BuildAst(const std::string& expr)
	{
		size_t brackets = 0;
		std::vector <AST> asts;

		FormAstLists(asts, expr, brackets); 
                
		while (brackets > 0) 
		{
			std::pair <size_t, size_t> closest_brackets = FindClosestBrackets(asts); 
			CreateAstBetweenBrackets(closest_brackets, asts); 
			--brackets; 
		}
		
		ConcatinateAsts(asts); 
		
		return asts[0]; 
	}

	void Ast_builder::FormAstLists(std::vector <AST>& asts, const std::string& expr, size_t& brackets)
	{	
		for (size_t i = 0; i < expr.size(); i++)
		{
		    std::string s;
		        
		    if (expr[i] == '{') 
		    {
		        i++;
		        while (expr[i] != '}' && expr[i] != ',') 
		        {
		            s += expr[i];
		            ++i;
		        }
		                
		        long long int k = stoll(s);
		        if (expr[i] == ',') 
		        {
		            s = "";
		            ++i;
		            while (expr[i] != '}')
		            {
		                s += expr[i];
		                ++i;
		            }
		            long long int v = stoul(s);
		                    
		            s ="{}";
		            asts.emplace_back(node_type::leaf, k, v, s); 
		            ++i;
		        }
		        else
		        {
		            s ="{}";
		            asts.emplace_back(node_type::leaf, k, s);
		            ++i;
                }
		                
		        if (i == expr.size()) break;
                else {
                    --i;
                    continue;
                }
		    }
		        
			if (expr[i] == '(') brackets++;

            s = expr[i];
			asts.emplace_back(node_type::leaf, s); 
		}

		if (asts.front().root->label != "(" || asts.back().root->label != ")") 
		{
			asts.emplace_back(node_type::leaf, ")");
			asts.emplace(asts.begin(), node_type::leaf, "(");
			++brackets;
		}
	}

	std::pair <size_t, size_t> Ast_builder::FindClosestBrackets(std::vector <AST>& asts) 
	{
		std::stack <size_t> open_brackets_indexes;
		std::pair <size_t, size_t> closest_pair = {0, asts.size()-1};

		for (size_t i = 0; i < asts.size(); i++)
		{
			if (asts[i].root->label == "(")
			{
				open_brackets_indexes.push(i);
			}
			else if (asts[i].root->label == ")")
			{
				size_t open = open_brackets_indexes.top();
				if ( (i - open) < (closest_pair.second - closest_pair.first) )
				{
					closest_pair.first = open;
					closest_pair.second = i;
				}
			}
		}

		return closest_pair;
	}

	void Ast_builder::CreateAstBetweenBrackets(std::pair <size_t, size_t>& closest_brackets, std::vector <AST>& asts)
	{
		for (size_t i = closest_brackets.first; i < closest_brackets.second; i++)
		{
			if (asts[i].root->left != nullptr) continue;


			if (asts[i].root->label == "+") 
			{
				AST new_ast = AST(node_type::positive_closure, "+");
				new_ast.SetLeftChild(asts[i-1].root);
				asts.erase(asts.begin() + i - 1);
				closest_brackets.second--;
				asts[i-1] = new_ast;
			}
		}
		
		size_t i = closest_brackets.first + 2;
		while (closest_brackets.second - closest_brackets.first != 2 && i < closest_brackets.second) 
		{
			if (asts[i].root->label == "{}")
			{
				AST next_ast = AST(node_type::repeat, asts[i].root->repeat_left, asts[i].root->repeat_right, "{}");
				next_ast.SetLeftChild(asts[i-1].root);
				asts[i-1] = next_ast;
				asts.erase(asts.begin() + i);
				closest_brackets.second--;
			}
			else i++;

		}

		i = closest_brackets.first + 2;
		while (closest_brackets.second - closest_brackets.first != 2 && i < closest_brackets.second) 
		{

			if (asts[i].root->label != "|" && asts[i-1].root->label != "|")
			{
				AST next_ast = AST(asts[i-1], asts[i], std::shared_ptr <Node> (new Node(node_type::concat, ".")));
				asts.erase(asts.begin() + i - 1);
				closest_brackets.second--;
				asts[i-1] = next_ast;
			}
			else if (asts[i].root->label == "|" && asts[i].root->type != node_type::leaf)
			{
			    AST next_ast = AST(asts[i-1], asts[i], std::shared_ptr <Node> (new Node(node_type::concat, ".")));
				asts.erase(asts.begin() + i - 1);
				closest_brackets.second--;
				asts[i-1] = next_ast;
			}
			else if (asts[i-1].root->label == "|" && asts[i-1].root->type != node_type::leaf)
			{
			    AST next_ast = AST(asts[i-1], asts[i], std::shared_ptr <Node> (new Node(node_type::concat, ".")));
				asts.erase(asts.begin() + i - 1);
				closest_brackets.second--;
				asts[i-1] = next_ast;
			}
			else ++i;

		}

		if (asts.size() != 1)
		{
			for (i = closest_brackets.first + 1; i < closest_brackets.second; i++)
			{
				if (asts[i].root->left != nullptr) continue;

				if (asts[i].root->label == "|")
				{
					AST next_ast = AST(asts[i-1], asts[i+1], std::shared_ptr <Node> (new Node(node_type::maybe, "|")));
					asts.erase(asts.begin() + i - 1);
					asts.erase(asts.begin() + i - 1);
					closest_brackets.second -= 2;
					asts[i-1] = next_ast;
					--i;
				}
			}
		}

		asts.erase(asts.begin() + closest_brackets.first);
		asts.erase(asts.begin() + closest_brackets.second - 1);
	}
        
	void Ast_builder::ConcatinateAsts(std::vector <AST>& asts)
	{
		while (asts.size() != 1)
		{
		    AST next_ast = AST(asts[0], asts[1], std::shared_ptr <Node> (new Node(node_type::concat, ".")));
			asts.erase(asts.begin());
			asts[0] = next_ast;
		}
	}
}
