#pragma once  

#include "../ast/ast.h"
#include "../ast_builder/ast_builder.h"
#include "../nfa/nfa.h"
#include "../dfa/dfa.h"
#include <vector>
#include <string>

using namespace Ast;
using namespace automats;

namespace regex
{
    class Regex
    {
    public:
        Regex(const std::string& reg_expr);
        size_t compile(int i = 0);
        void find_all(const std::vector<std::string> &strings, std::vector<std::string> &ans);
        Regex invert() const;
        void DfaPrint(int i = 0);
        std::string recover() const;
                  
    private:
        std::string reg_expr;
                        
        AST ast;
        Nfa nfa;
        Dfa dfa;
    };
}
