#pragma once
#include <vector>
#include <set>
#include <tuple>
#include <string>
#include <iostream>
#include <stack>
#include "PostfixForm.h"

struct stare
{
};

class NedeterministicFiniteAutomaton
{
public:
	
	NedeterministicFiniteAutomaton() : m_initialState{ 0 } {};
	friend class DeterministicFiniteAutomaton;


	NedeterministicFiniteAutomaton BuildAutomaton(const std::string& Expression);
	void PrintAutomaton()const;
private:
	std::vector<int> m_states; 
	std::vector<char> m_inputAlphabet; 
	int m_initialState; 
	std::vector<int> m_finalStates; 
	std::set<std::tuple<int, char, int>> m_transitionFunction; 


};