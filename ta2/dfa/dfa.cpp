#include "dfa.h"

namespace automats
{
    void Dfa::print()
    {
        std::cout << "Determ Automat:" << std::endl;
	    std::cout << this->states.size() << std::endl;

	    for (size_t i = 0; i < states.size(); i++)
	    {
		    std::cout << "State: " << i << " " << this->states[i]->is_terminal << std::endl;
		    for (const auto& elem : this->states[i]->id_transitions) {
			    std::cout << elem.first << " " << elem.second << std::endl;
		    }
	    }
		
	    std::cout << "Terminal states:" << std::endl;
	    for (size_t i = 0; i < this->terminal_states_id.size(); i++) {
		    std::cout << this->terminal_states_id[i] << " ";
	    }
	    std::cout << std::endl;
		
	    std::cout << "Start state: " << this->start_state_id << std::endl;
    }
        
    Dfa Dfa::FormDfa(std::shared_ptr <Nfa> nfa)
    {
        Dfa dfa;

        std::map <std::set <size_t>, size_t> dfa_state_map;
        std::queue <std::set <size_t> > states_queue;
        std::vector <std::shared_ptr <Nfa::State> > &nfa_states = nfa->states;


        std::set <size_t> start_set(nfa->start_states.begin(), nfa->start_states.end());
                
        std::set<size_t> start_epsilon_closure = GetEpsilonClosure(nfa, start_set);

        size_t dfa_start_state_id = dfa.states.size();
        dfa.start_state_id = dfa_start_state_id;
        dfa_state_map[start_epsilon_closure] = dfa_start_state_id;
        dfa.states.push_back(std::make_shared <State> (false));
        states_queue.push(start_epsilon_closure);

        while (!states_queue.empty())
        {
            std::set<size_t> curr_nfa_states = states_queue.front();
            states_queue.pop();
            size_t dfa_state_id = dfa_state_map[curr_nfa_states];

            std::map <std::string, std::set <size_t> > transitions;

            for (size_t nfa_state_id : curr_nfa_states)
            {
                for (const auto &transition : nfa_states[nfa_state_id]->id_transitions)
                {
                    const std::string &symbol = transition.first;
                    if (symbol == "$") continue;


                    if (transitions.find(symbol) == transitions.end()) {
                        transitions[symbol] = std::set <size_t> ();
                    }

                    for (size_t next_state : transition.second) {
                        transitions[symbol].insert(next_state);
                    }
                }
            }

            for (const auto &transition : transitions)
            {
                std::string symbol = transition.first;
                                
                std::set <size_t> next_states = GetEpsilonClosure(nfa, transition.second);

                if (dfa_state_map.find(next_states) == dfa_state_map.end())
                {
                    size_t new_dfa_state_id = dfa.states.size();
                    dfa_state_map[next_states] = new_dfa_state_id;
                    dfa.states.push_back(std::make_shared <State> (false));
                    states_queue.push(next_states);
                }

                dfa.states[dfa_state_id]->id_transitions[symbol] = dfa_state_map[next_states];
            }   
        }

        for (const auto &pair : dfa_state_map)
        {
            const std::set <size_t>& nfa_states_set = pair.first;
            size_t dfa_state_id = pair.second;

            for (size_t nfa_state_id : nfa_states_set)
            {
                if (nfa_states[nfa_state_id]->is_terminal)
                {
                    dfa.states[dfa_state_id]->is_terminal = true;
                                        
                    dfa.terminal_states_id.push_back(dfa_state_id);
                    break;
                }
            }
        }

        return dfa;
    }

    std::set <size_t> Dfa::GetEpsilonClosure(std::shared_ptr <Nfa> nfa, std::set <size_t> curr_states)
    {
        std::set <size_t> epsilon_closure = curr_states;
        std::queue <size_t> states_queue;

        for (size_t state : curr_states) {
            states_queue.push(state);
        }

        while (!states_queue.empty())
        {
            size_t state = states_queue.front();
            states_queue.pop();

            auto transitions = nfa->states[state]->id_transitions;
            if (transitions.find("$") != transitions.end())
            {
                for (size_t next_state : transitions["$"])
                {
                    if (epsilon_closure.find(next_state) == epsilon_closure.end())
                    {
                        epsilon_closure.insert(next_state);
                        states_queue.push(next_state);
                    }
                }
            }
        }

        return epsilon_closure;
    }

    void Dfa::MinimizeDfa()
    { 
        if (states.empty()) return;

        std::vector<std::string> alphabet;
        std::set<std::string> alpha_set;
        for (auto &st : states)
        {
            for (auto &tr : st->id_transitions)
            {
                alpha_set.insert(tr.first);
            }
        }
        alphabet.assign(alpha_set.begin(), alpha_set.end());

        std::vector <std::set <size_t> > partitions;
        std::set<size_t> term, nonterm;
        for (size_t i = 0; i < states.size(); i++)
        {
            (states[i]->is_terminal ? term : nonterm).insert(i);
        }
        if (!nonterm.empty()) partitions.push_back(std::move(nonterm));
        if (!term.empty())     partitions.push_back(std::move(term));
            
        std::vector <int> state_block(states.size(), -1);
        bool changed = true;
        while (changed)
        {
            
            for (size_t b = 0; b < partitions.size(); b++)
            {
                for (auto s : partitions[b])
                {
                    state_block[s] = int(b);
                }
            }

            changed = false;
            std::vector <std::set <size_t> > new_parts;

            for (auto& grp : partitions)
            {
                if (grp.size() <= 1)
                {
                    new_parts.push_back(grp);
                    continue;
                }

                std::map<std::vector<int>, std::set<size_t>> splitter;
                for (auto s : grp)
                {
                    std::vector <int> sig;
                    sig.reserve(alphabet.size());
                    for (auto &sym : alphabet)
                    {
                        auto it = states[s]->id_transitions.find(sym);
                            
                        if (it != states[s]->id_transitions.end()) sig.push_back(state_block[it->second]);
                        else sig.push_back(-1);

                    }
                    splitter[sig].insert(s);
                }

                if (splitter.size() == 1) new_parts.push_back(grp);

                else
                {     
                    for (auto &kv : splitter) new_parts.push_back(std::move(kv.second));

                    changed = true;
                }
            }

            partitions.swap(new_parts);
        }

        
        std::map <size_t, size_t> old2new;
        std::vector <std::shared_ptr<State> > new_states;
        std::vector <size_t> new_terminals;
        size_t new_start = 0;

        for (size_t b = 0; b < partitions.size(); b++)
        {
            auto& grp = partitions[b];
            if (grp.empty()) continue;

            size_t repr = *grp.begin();
            bool isT = states[repr]->is_terminal;
            new_states.push_back(std::make_shared<State>(isT));
                
            if (isT) new_terminals.push_back(b);

            for (auto s : grp) old2new[s] = b;

            if (grp.count(start_state_id)) new_start = b;

        }

        for (size_t b = 0; b < partitions.size(); b++)
        {
            auto& grp = partitions[b];
                
            if (grp.empty()) continue;

            size_t repr = *grp.begin();
                
            for (auto& tr : states[repr]->id_transitions) {
                new_states[b]->id_transitions[tr.first] = old2new[tr.second];
            }
        }

        states.swap(new_states);
        terminal_states_id.swap(new_terminals);
        start_state_id = new_start;
        curr_state = new_start;
    }
        
    void Dfa::SetCurrStateOnStart()
    {
        this->curr_state = this->start_state_id;
    }
        
    Dfa Dfa::invert() const
    {
            
        auto rev_nfa = std::make_shared<Nfa>();
  
        size_t origN = this->states.size();

        rev_nfa->states.reserve(origN + 2);
        for (size_t i = 0; i < origN + 2; i++) {
            rev_nfa->states.push_back(std::make_shared<Nfa::State>(false));
        }
        size_t newAccept = origN + 1;
        rev_nfa->states[newAccept]->is_terminal = true;

        rev_nfa->start_states   = { 0 };
        rev_nfa->terminal_states= { newAccept };

        for (size_t p = 0; p < origN; p++)
        {
            for (auto& tr : this->states[p]->id_transitions)
            {
                const std::string &sym = tr.first;
                size_t q = tr.second;

                rev_nfa->states[q+1]->id_transitions[sym].push_back(p+1);
                rev_nfa->states[q+1]->transitions_strings.insert(sym);
            }
        }

        for (size_t t : this->terminal_states_id) {
            rev_nfa->states[0]->id_transitions["$"].push_back(t+1);
            rev_nfa->states[0]->transitions_strings.insert("$");
        }

        rev_nfa->states[this->start_state_id + 1]
            ->id_transitions["$"].push_back(newAccept);
        rev_nfa->states[this->start_state_id + 1]
            ->transitions_strings.insert("$");
   
        Dfa reversed = Dfa::FormDfa(rev_nfa);
        return reversed;
    }
        
    size_t Dfa::DoTransition(const char& c) 
    {
        std::string s;
        s += c;
        for (const auto &transition : states[this->curr_state]->id_transitions)
        {
            if (transition.first == s)
            {
                this->curr_state = transition.second;            
                if (this->CheckCurrStateIsTerminal()) return 0;
                return 1;
            }
        }
        return 2;
    }
        
    bool Dfa::CheckCurrStateIsTerminal()
    {
        return this->states[this->curr_state]->is_terminal;
    }
        
    void Dfa::ToGraphviz(const std::string& filename) const
    {
        std::ofstream out(filename);
        if (!out)
        {
            std::cerr << "Не удалось открыть файл " << filename << " для записи DOT\n";
            return;
        }

        out << "digraph DFA {\n";
        out << "    rankdir=LR;\n\n";

        out << "    node [shape=doublecircle];";
        for (size_t i = 0; i < states.size(); i++)
        {
            if (states[i]->is_terminal)
                out << " q" << i;
        }
        out << ";\n\n";

        out << "    node [shape=circle];\n\n";

        out << "    init [shape=point];\n";
        out << "    init -> q" << start_state_id << ";\n\n";

        for (size_t i = 0; i < states.size(); i++)
        {
            for (const auto& tr : states[i]->id_transitions)
            {
                out << "    q" << i
                    << " -> q" << tr.second
                    << " [label=\"" << tr.first << "\"];"
                    << "\n";
            }
        }

        out << "}\n";
    }
    
    int Dfa::GetNumStates() const
    {
        return static_cast <int> (states.size());
    }
    
    size_t Dfa::GetStartState() const
    {
        return start_state_id;
    }
    
    std::vector <size_t> Dfa::GetFinalStates() const
    {
        return terminal_states_id;
    }
    
    std::vector <std::string> Dfa::GetTransitionLabels(size_t from, size_t to) const
    {
        std::vector<std::string> labels;
        
        const auto &tr_map = states.at(from)->id_transitions;
        for (const auto& kv : tr_map)
        {
            if (kv.second == to) labels.push_back(kv.first);

        }
        return labels;
    }
}
