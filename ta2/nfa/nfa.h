#pragma once

#include <map>
#include <memory>
#include <vector>
#include <iostream>
#include <set>
#include <algorithm>
#include "../ast/ast.h"

namespace automats
{
	class Nfa
	{
	    friend class Dfa;
	      
	public:
		Nfa() = default;

		Nfa(std::string s);

		Nfa(const Nfa& other_nfa);

		static Nfa TompsonFormAutomat(std::shared_ptr <Ast::Node> tree);

		void NfaPrint();

		void ToGraphviz(const std::string& filename = "nfa.dot") const;

	protected:
		struct State
		{
			bool is_terminal;
			std::map <std::string, std::vector <size_t> > id_transitions;
			std::set <std::string> transitions_strings;

			State(bool is_terminal) : is_terminal(is_terminal) {}
			State(bool is_terminal, std::string transition);
			State(bool is_terminal, std::string transition, std::shared_ptr <State> st);
			State(const State& st);
		};

		std::vector <size_t> start_states;
		std::vector <std::shared_ptr <State> > states;
		std::vector <size_t> terminal_states;
			
		Nfa CreateTompsonPositiveClosureAutomat(Nfa& other);

		Nfa CreateTompsonAddAutomat(Nfa& other);

		Nfa CreateTompsonOrAutomat(Nfa& other);
	};	
}
