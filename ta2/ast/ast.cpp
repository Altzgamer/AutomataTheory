#include "ast.h"
#include <iomanip>

namespace Ast
{
    std::string CastNodeTypeToString(node_type type)
    {
        if (type == node_type::root)  return "root";

        else if (type == node_type::leaf) return "leaf";

        else if (type == node_type::concat) return "concat";

        else if (type == node_type::positive_closure) return "positive_closure";

        else if (type == node_type::maybe) return "maybe";

        else if (type == node_type::repeat) return "repeat";

        return "leaf";
    }
    
	AST::AST(AST& left_tree, AST& right_tree, std::shared_ptr <Node> node) : root(node)
	{
		this->root->left = left_tree.root;
		this->root->right = right_tree.root;
	}
	
	AST& AST::SetLeftChild(std::shared_ptr <Node> left_node)
	{
	    this->root->left = left_node;
	    return *this;
    }

    int AST::DfsPrint(const std::shared_ptr <Node> & node, int& id_counter, std::ostream& out)
    {
        if (!node) return -1;

        int my_id = id_counter++;

        out << "  node" << my_id
            << " [label=\"" << CastNodeTypeToString(node->type) << "\\n";
            
        if (node->type == node_type::repeat)
        {
            if (node->repeat_right == -1)
            {
                out << "\\ncount=" << node->repeat_left;
            }
            else
            {
                out << "\\ncount_left=" << node->repeat_left << "\\ncount_right=" << node->repeat_right;
            }
        }
            
        if (!node->label.empty())
        {
            out << "\\n" << node->label;
        } 
            
        out << "\"]" << ";\n";

        if (node->left) {
            int left_id = DfsPrint(node->left, id_counter, out);
            out << "  node" << my_id << " -> node" << left_id << ";\n";
        }

        if (node->right) {
            int right_id = DfsPrint(node->right, id_counter, out);
            out << "  node" << my_id << " -> node" << right_id << ";\n";
        }

        return my_id;
    }

    void AST::print(const std::string& filename)
    {
        std::ofstream out(filename);
        if (!out.is_open())
        {
            throw std::runtime_error("Не удалось открыть файл для записи: " + filename);
        }

        out << "digraph AST {\n";
        int counter = 0;
        DfsPrint(this->root, counter, out);
        out << "}\n";
    }
}
