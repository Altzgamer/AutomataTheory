#pragma once

#include <map>
#include <memory>
#include <vector>
#include <iostream>
#include <queue>
#include <set>
#include <algorithm>
#include "../nfa/nfa.h"

namespace automats
{
    class Dfa
    {       
    public:
		
		Dfa() = default;
		Dfa(std::string s);
		Dfa(const Dfa& other_dfa);
			
		static Dfa FormDfa(std::shared_ptr <Nfa> nfa);
		void print();
		void MinimizeDfa();
		Dfa invert() const;
			
		void SetCurrStateOnStart();
		size_t DoTransition(const char& c);
		bool CheckCurrStateIsTerminal();
		void ToGraphviz(const std::string& filename = "dfa.dot") const;
		int GetNumStates() const;
		size_t GetStartState() const;
		std::vector <size_t> GetFinalStates() const;
		std::vector<std::string> GetTransitionLabels(size_t from, size_t to) const;

	private:
		struct State
		{
			bool is_terminal;
			std::map <std::string, size_t> id_transitions; 

			State(bool is_terminal) : is_terminal(is_terminal) {}
			State(bool is_terminal, std::string transition);
			State(bool is_terminal, std::string transition, std::shared_ptr <State> st);
			State(const State& st);
		};

			
		std::shared_ptr <State> start_state;
		size_t start_state_id; 
			
		size_t curr_state;
			
		std::vector <std::shared_ptr <State> > states;
			
		std::vector <size_t> terminal_states_id; 
			
		static std::set <size_t> GetEpsilonClosure(std::shared_ptr <Nfa> nfa, std::set <size_t> curr_states);
    };
}
