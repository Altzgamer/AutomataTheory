#include "my_regex.h"

namespace regex
{
    Regex::Regex(const std::string& reg_expr)
    {
        this->reg_expr = reg_expr;
    }
        
    size_t Regex::compile(int i)
    {
        this->ast = Ast_builder::BuildAst(this->reg_expr);
        this->ast.print("ast_" + std::to_string(i) + ".dot");

	    this->nfa = automats::Nfa::TompsonFormAutomat(ast.root);
	    this->nfa.ToGraphviz("nfa_" + std::to_string(i) + ".dot");
	
	    std::shared_ptr <automats::Nfa> p {std::make_shared <Nfa> (this->nfa)};
	
	    this->dfa = Dfa::FormDfa(p);
	    this->dfa.ToGraphviz("dfa_" + std::to_string(i) + ".dot");
	
	    this->dfa.MinimizeDfa();
	    this->dfa.ToGraphviz("dfa_minimize_" + std::to_string(i) + ".dot");
	    return 0;
    }

    void Regex::find_all(const std::vector <std::string> &strings,
                         std::vector <std::string> &ans)
    {
        for (const auto &s : strings)
        {
            size_t i = 0;
            while (i < s.size())
            {
                
                this->dfa.SetCurrStateOnStart();

                size_t lastMatch = std::string::npos;

                for (size_t j = i; j < s.size(); j++)
                {
                    size_t code = this->dfa.DoTransition(s[j]);
                    if (code == 2) break;
                    if (code == 0) lastMatch = j;
                }

                if (lastMatch != std::string::npos)
                { 
                    ans.push_back(s.substr(i, lastMatch - i + 1));
                    
                    i = lastMatch + 1;
                }
                else ++i;

            }
        }
    }
        
    Regex Regex::invert() const
    {

        Dfa inv_dfa = this->dfa.invert();


        Regex result("");
        result.dfa = std::move(inv_dfa);
        result.dfa.print();
        return result;
    }
    void Regex::DfaPrint(int i)
    {
        this->dfa.ToGraphviz("dfa_minimize_" + std::to_string(i) + ".dot");
    }
    
    static std::string unite(const std::string &a, const std::string &b)
    {
        if (a.empty())
        {
            return b;
        }
        if (b.empty())
        {
            return a;
        }
        return "(" + a + "|" + b + ")";
    }

// Конкатенация двух regex, пустая строка ε
    static std::string concat(const std::string& a, const std::string& b)
    {
        if (a.empty()) return b;
        if (b.empty()) return a;
        return a + b;
    }

    static std::string plus(const std::string &a)
    {
        if (a.empty()) return "";
        if (a.size() == 1) return a + "+";
        return "(" + a + ")+";
    }

    std::string Regex::recover() const
    {

        int N = dfa.GetNumStates();
        int start = dfa.GetStartState();
        std::vector<bool> is_final(N, false);
        for (int f : dfa.GetFinalStates())
        {
            is_final[f] = true;
        }


        std::vector<std::vector<std::vector<std::string>>> R(
            N+1,
            std::vector<std::vector<std::string>>(N, std::vector<std::string>(N))
        );

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++) {
                
                auto syms = dfa.GetTransitionLabels(i, j);
                std::string r;
                for (auto const& lbl : syms)
                {
                    
                    r = unite(r, lbl);
                }
                
                if (i == j)
                {
                    r = unite(r, "");
                }
                R[0][i][j] = r;
            }
        }

        
        for (int k = 1; k <= N; k++)
        {
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    
                    std::string r = R[k-1][i][j];
                    
                    const std::string &rik = R[k-1][i][k-1];
                    const std::string &rkk = R[k-1][k-1][k-1];
                    const std::string &rkj = R[k-1][k-1][j];
                    if (!rik.empty() && !rkj.empty())
                    {
                        
                        std::string middle = concat(rik, concat(plus(rkk), rkj));
                        r = unite(r, middle);
                    }
                    R[k][i][j] = r;
                }
            }
        }

        std::string result;
        for (int f = 0; f < N; f++)
        {
            if (!is_final[f]) continue;
            result = unite(result, R[N][start][f]);
        }
        std::cout << result << std::endl;
        return result;
    }
}
