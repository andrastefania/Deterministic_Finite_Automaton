#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <stack>
#include <fstream>
#include "NedeterministicFiniteAutomaton.h"
#include <set>
#include <map>
#include<queue>
class DeterministicFiniteAutomaton
{
public:
	DeterministicFiniteAutomaton() : m_initialStateAFD(-1) {}
	std::set<int> LambdaClosure(const NedeterministicFiniteAutomaton& afn, const std::set<int>& states);
	std::set<int> Delta(const NedeterministicFiniteAutomaton& afn, const std::set<int>& currentStates, char symbol);
	
	DeterministicFiniteAutomaton ConvertIntoAFD(const std::string & expression);
	bool VerifyAutomaton() const;
	void PrintAutomaton() const;
	bool CheckWord(const std::string& word) const;
	void WriteToFile(const std::string& fileName);
public:
	std::vector<int> m_statesAFD; 
	std::vector<char> m_inputAlphabetAFD; 
	int m_initialStateAFD; 
	std::vector<int> m_finalStatesAFD; 
	std::set<std::tuple<int, char, int>> m_transitionFunctionAFD;

};