#include "nfa.h"

namespace automats
{       
    Nfa Nfa::TompsonFormAutomat(std::shared_ptr <Ast::Node> tree)
{
        if (tree->left != nullptr)
        {
            Nfa left_automat = TompsonFormAutomat(tree->left);
            if (tree->right != nullptr)
            {
                                
                Nfa right_automat = TompsonFormAutomat(tree->right);
                        
                if (tree->type == Ast::node_type::maybe) {
				    Nfa res_automat = left_automat.CreateTompsonOrAutomat(right_automat);
				    return res_automat;
			    }
			    else if (tree->type == Ast::node_type::concat) {
                    Nfa res_automat = left_automat.CreateTompsonAddAutomat(right_automat);
                    return res_automat;
                }
            }
            else {
                if (tree->type == Ast::node_type::positive_closure) return left_automat.CreateTompsonPositiveClosureAutomat(left_automat);

                else if (tree->type == Ast::node_type::repeat) {
                    
                    long long int m = tree->repeat_left;
                    long long int n = tree->repeat_right; 
                       
                    Nfa unit = left_automat;
        
                    Nfa res;
                    if (m > 0) {
                        res = unit;
                        for (long long int i = 1; i < m; i++) {
                            res = res.CreateTompsonAddAutomat(unit);
                        }
                    }
                    else {   
                        Nfa eps;
                        eps.states.emplace_back(std::make_shared<State>(false));
                        eps.states.emplace_back(std::make_shared<State>(true));
                        eps.start_states   = {0};
                        eps.terminal_states= {1};
                        eps.states[0]->id_transitions["$"].push_back(1);
                        eps.states[0]->transitions_strings.insert("$");
                        res = eps;
                    }
        
                    if (n >= 0 && n > m)
                    {
                        
                        Nfa eps;
                        eps.states.emplace_back(std::make_shared<State>(false));
                        eps.states.emplace_back(std::make_shared<State>(true));
                        eps.start_states   = {0};
                        eps.terminal_states= {1};
                        eps.states[0]->id_transitions["$"].push_back(1);
                        eps.states[0]->transitions_strings.insert("$");
        
                        for (long long int k = 0; k < n - m; k++) {
                            
                            Nfa opt = unit.CreateTompsonOrAutomat(eps);
                            
                            res = res.CreateTompsonAddAutomat(opt);
                        }
                    }
        
                    return res;
                }
            }
        }
                
        Nfa automat(tree->label);
        return automat;
    }
        
    Nfa::Nfa(std::string s)
    {
        states.emplace_back(std::make_shared <State> (false));
        states.emplace_back(std::make_shared <State> (true));
                
        states[0]->id_transitions[s].push_back(1);
        states[0]->transitions_strings.insert(s);
                
        start_states = {0};
        terminal_states = {1};
    }
        
    Nfa Nfa::CreateTompsonPositiveClosureAutomat(Nfa& other)
    {
        Nfa res;
        
        res.states.emplace_back(std::make_shared <State> (false));
        
        size_t off = res.states.size();
                
        for (auto& st : other.states) {
            res.states.push_back(std::make_shared <State> (st->is_terminal));
        }
                
        for (size_t i = 0; i < other.states.size(); i++)
        {
            for (auto& kv : other.states[i]->id_transitions)
            {
                for (auto dst : kv.second)
                {
                    res.states[i+off]->id_transitions[kv.first].push_back(dst+off);
                    res.states[i+off]->transitions_strings.insert(kv.first);
                }
            }
        }
        
        res.states.emplace_back(std::make_shared <State> (true));
        size_t new_accept = res.states.size() - 1;
        
        res.states[0]->id_transitions["$"].push_back(off + other.start_states[0]);
        res.states[0]->transitions_strings.insert("$");
        
        for (auto t : other.terminal_states)
        {
            size_t old_acc = off + t;
            res.states[old_acc]->id_transitions["$"].push_back(off + other.start_states[0]);
            res.states[old_acc]->id_transitions["$"].push_back(new_accept);
            res.states[old_acc]->transitions_strings.insert("$");
        }
        
        res.start_states   = {0};
        res.terminal_states = {new_accept};
                
        for (auto &st : res.states) st->is_terminal = false;
        for (auto t : res.terminal_states) res.states[t]->is_terminal = true;
     
        return res;
    }

	Nfa Nfa::CreateTompsonOrAutomat(Nfa& other)
	{
	    Nfa res;
                
        res.states.emplace_back(std::make_shared <State> (false));
        
        size_t off1 = res.states.size();
        for (auto& st : this->states) {
            res.states.push_back(std::make_shared <State> (st->is_terminal));
        }

        for (size_t i = 0; i < this->states.size(); i++)
        {
            for (auto& kv : this->states[i]->id_transitions)
            {
                for (auto dst : kv.second)
                {
                    res.states[i+off1]->id_transitions[kv.first].push_back(dst+off1);
                    res.states[i+off1]->transitions_strings.insert(kv.first);
                }
            }
        }
        
        size_t off2 = res.states.size();
                
        for (auto& st : other.states) {
            res.states.push_back(std::make_shared <State> (st->is_terminal));
        }
                
        for (size_t i = 0; i < other.states.size(); i++)
        {
            for (auto& kv : other.states[i]->id_transitions)
            {
                for (auto dst : kv.second)
                {
                    res.states[i+off2]->id_transitions[kv.first].push_back(dst+off2);
                    res.states[i+off2]->transitions_strings.insert(kv.first);
                }
            }
        }
                
        res.states.emplace_back(std::make_shared <State>(true));
        size_t new_accept = res.states.size() - 1;

        res.states[0]->id_transitions["$"].push_back(off1 + this->start_states[0]);
        res.states[0]->id_transitions["$"].push_back(off2 + other.start_states[0]);
        res.states[0]->transitions_strings.insert("$");

        for (auto t : this->terminal_states)
        {
            res.states[off1 + t]->id_transitions["$"].push_back(new_accept);
            res.states[off1 + t]->transitions_strings.insert("$");
        }
        for (auto t : other.terminal_states)
        {
            res.states[off2 + t]->id_transitions["$"].push_back(new_accept);
            res.states[off2 + t]->transitions_strings.insert("$");
        }

        res.start_states   = {0};
        res.terminal_states = {new_accept};
                
        for (auto &st : res.states) st->is_terminal = false;
        for (auto t : res.terminal_states) res.states[t]->is_terminal = true;

                
        return res;
	}
	
	Nfa Nfa::CreateTompsonAddAutomat(Nfa& other)
	{
        Nfa res = *this; 
        size_t offset = res.states.size();

        for (auto& st : other.states) {
            res.states.push_back(std::make_shared <State> (st->is_terminal));
        }

        for (size_t i = 0; i < other.states.size(); i++)
        {
            for (auto& kv : other.states[i]->id_transitions)
            {
                for (auto dst : kv.second)
                {
                    res.states[i+offset]->id_transitions[kv.first].push_back(dst+offset);
                    res.states[i+offset]->transitions_strings.insert(kv.first);
                }
            }
        }
        
        for (auto t : res.terminal_states)
        {
            res.states[t]->id_transitions["$"].push_back(offset + other.start_states[0]);
            res.states[t]->transitions_strings.insert("$");
        }
        
        res.start_states = this->start_states;
        res.terminal_states.clear();

        for (auto t : other.terminal_states) res.terminal_states.push_back(t + offset);
        for (auto &st : res.states) st->is_terminal = false;
        for (auto t : res.terminal_states) res.states[t]->is_terminal = true;

        return res;
    }

	Nfa::Nfa(const Nfa& other_nfa)
	{
		this->states.resize(other_nfa.states.size());
		for (size_t i = 0; i < this->states.size(); i++)
		{
			this->states[i] = std::shared_ptr <State> (new State(other_nfa.states[i]->is_terminal));
		}

		for (size_t i = 0; i < this->states.size(); i++)
		{
			for (const auto& elem : other_nfa.states[i]->id_transitions) {
				this->states[i]->id_transitions[elem.first] = elem.second;
			}
			
			this->states[i]->transitions_strings = other_nfa.states[i]->transitions_strings;
		}
		
		this->terminal_states.resize(other_nfa.terminal_states.size());
		for (size_t i = 0; i < this->terminal_states.size(); i++) {
			this->terminal_states[i] = other_nfa.terminal_states[i];
		}
		
		this->start_states.push_back(0);
	}

	void Nfa::NfaPrint()
	{
		std::cout << "Automat:" << std::endl;
		std::cout << this->states.size() << std::endl;
		for (size_t i = 0; i < states.size(); i++)
		{
			std::cout << "State: " << i << " " << this->states[i]->is_terminal << std::endl;
			for (const auto& elem : this->states[i]->id_transitions)
			{
			    std::cout << elem.first << " ";
				for (auto& transition : elem.second)
				{
				    std::cout << transition << " ";
				}
				std::cout << std::endl;
			}
		}
		
		std::cout << "Terminal states:" << std::endl;
		for (size_t i = 0; i < terminal_states.size(); i++) {
		    std::cout << terminal_states[i] << " ";
		}
		std::cout << std::endl;
	}
	
	void Nfa::ToGraphviz(const std::string& filename) const
        {
        std::ofstream out(filename);
            
        if (!out.is_open())
        {
            std::cerr << "Error: cannot open file " << filename << " for writing\n";
            return;
        }

        out << "digraph NFA {\n";
        out << "    rankdir=LR;\n";
        out << "    node [shape = circle];\n\n";

        for (size_t t : terminal_states)
        {
            out << "    " << t << " [shape = doublecircle];\n";
        }
        out << "\n";

        for (size_t i = 0; i < states.size(); i++)
        {
            const auto& st = states[i];
            for (const auto& kv : st->id_transitions)
            {
                const std::string& sym = kv.first == "$" ? "ε" : kv.first;
                for (size_t target : kv.second)
                {
                    out << "    " << i << " -> " << target
                        << " [ label = \"" << sym << "\" ];\n";
                }
            }
        }

        out << "}\n";
        out.close();
        std::cout << "Graphviz DOT written to " << filename << std::endl;
        }
}
