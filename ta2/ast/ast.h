#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace Ast
{

	enum class node_type
	{
	    root,
		leaf,
		repeat,
		maybe,
		concat,
		positive_closure
	};

	std::string CastNodeTypeToString(node_type type);

	struct Node
	{
		node_type type; 
		std::string label; 
		long long int repeat_left; 
		long long int repeat_right; 
		std::shared_ptr <Node> left;
		std::shared_ptr <Node> right;
		
		Node(node_type type, const std::string& label = "") : type(type), label(label), repeat_left(-1), repeat_right(-1), left(nullptr), right(nullptr)  {}

		Node(node_type type, long long int repeat_left, const std::string& label = "") : type(type), label(label), repeat_left(repeat_left), repeat_right(-1), left(nullptr), right(nullptr)  {}

		Node(node_type type, long long int repeat_left, long long int repeat_right, const std::string& label = "") : type(type), label(label), repeat_left(repeat_left), repeat_right(repeat_right), left(nullptr), right(nullptr)  {}

	};

	class AST
	{
	public:
		std::shared_ptr <Node> root;
			
		AST() : root(nullptr) {}
		AST(node_type type, const std::string& label) : root(new Node(type, label)) {}
		AST(node_type type, long long int repeat_left, const std::string& label) : root(new Node(type, repeat_left, label)) {} 
		AST(node_type type, long long int repeat_left, long long int repeat_right, const std::string& label) : root(new Node(type, repeat_left, repeat_right, label)) {} 
		AST(AST& left_tree, AST& right_tree, std::shared_ptr <Node> node);
		AST& SetLeftChild(std::shared_ptr <Node> left_node);
		void print(const std::string& filename = "ast.dot");

			
	private:

	    int DfsPrint(const std::shared_ptr <Node>& node, int& id_counter, std::ostream& out);
	};
}

