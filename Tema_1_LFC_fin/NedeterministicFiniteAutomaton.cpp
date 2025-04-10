#include "NedeterministicFiniteAutomaton.h"

NedeterministicFiniteAutomaton NedeterministicFiniteAutomaton::BuildAutomaton(const std::string& expression)
{
	PostfixForm exp;
	std::string postfixExpression = exp.Conversion(expression);
	
	std::stack <NedeterministicFiniteAutomaton> automatonStack;
	int stateCounter = 0;


	for (char ch : postfixExpression)
	{
		if (isalpha(ch) || isdigit(ch))
		{
			NedeterministicFiniteAutomaton automaton;
			automaton.m_states.push_back(stateCounter);
			automaton.m_states.push_back(stateCounter+1);
			automaton.m_inputAlphabet.push_back(ch);
			automaton.m_finalStates.push_back(stateCounter+1);
			automaton.m_initialState = stateCounter;
			automaton.m_transitionFunction.insert({stateCounter, ch, stateCounter+1});
			stateCounter += 2;

			automatonStack.push(automaton);

		}
		else if (ch == '|') {
			NedeterministicFiniteAutomaton B = automatonStack.top(); automatonStack.pop();
			NedeterministicFiniteAutomaton A = automatonStack.top(); automatonStack.pop();
			NedeterministicFiniteAutomaton C;

			C.m_initialState = stateCounter;
			C.m_finalStates.push_back(stateCounter + 1);

			C.m_states.push_back(C.m_initialState);
			C.m_states.insert(C.m_states.end(), A.m_states.begin(), A.m_states.end());
			C.m_states.insert(C.m_states.end(), B.m_states.begin(), B.m_states.end());
			C.m_states.push_back(C.m_finalStates[0]);

			std::set<char> inputSet(A.m_inputAlphabet.begin(), A.m_inputAlphabet.end());
			inputSet.insert(B.m_inputAlphabet.begin(), B.m_inputAlphabet.end());
			C.m_inputAlphabet.assign(inputSet.begin(), inputSet.end());

			C.m_transitionFunction.insert(A.m_transitionFunction.begin(), A.m_transitionFunction.end());
			C.m_transitionFunction.insert(B.m_transitionFunction.begin(), B.m_transitionFunction.end());

			C.m_transitionFunction.insert({ C.m_initialState, 'l', A.m_initialState });
			C.m_transitionFunction.insert({ C.m_initialState, 'l', B.m_initialState });

			for (int finalState : A.m_finalStates) {
				C.m_transitionFunction.insert({ finalState, 'l', C.m_finalStates[0] });
			}
			for (int finalState : B.m_finalStates) {
				C.m_transitionFunction.insert({ finalState, 'l', C.m_finalStates[0] });
			}

			stateCounter += 2;
			automatonStack.push(C);
		}

		else if (ch == '.')
		{
			NedeterministicFiniteAutomaton B = automatonStack.top(); automatonStack.pop();
			NedeterministicFiniteAutomaton A = automatonStack.top(); automatonStack.pop();
			NedeterministicFiniteAutomaton C;
			for (auto state : A.m_states)
				C.m_states.push_back(state);
			C.m_states.insert(C.m_states.end(), B.m_states.begin(), B.m_states.end());
			
			for (char symbol : A.m_inputAlphabet)
				C.m_inputAlphabet.push_back(symbol);
			for (char symbol : B.m_inputAlphabet) {
				if (std::find(C.m_inputAlphabet.begin(), C.m_inputAlphabet.end(), symbol) == C.m_inputAlphabet.end()) {
					C.m_inputAlphabet.push_back(symbol);
				}
			}
			C.m_transitionFunction = A.m_transitionFunction;
			C.m_transitionFunction.insert(B.m_transitionFunction.begin(), B.m_transitionFunction.end());
			for (int finalStateA : A.m_finalStates) {
				C.m_transitionFunction.insert({ finalStateA, 'l', B.m_initialState });
			}
			C.m_initialState = A.m_initialState;
			C.m_finalStates = B.m_finalStates;

			automatonStack.push(C);
		}
		else if (ch == '*')
		{
			NedeterministicFiniteAutomaton A = automatonStack.top(); automatonStack.pop();
			NedeterministicFiniteAutomaton C;
			C.m_initialState = stateCounter++;
			int newFinalState = stateCounter++;

			for (auto state : A.m_states)
				C.m_states.push_back(state);

			C.m_states.push_back(C.m_initialState);
			C.m_states.push_back(newFinalState);

			for (char symbol : A.m_inputAlphabet)
				C.m_inputAlphabet.push_back(symbol);
			C.m_transitionFunction = A.m_transitionFunction;
			C.m_transitionFunction.insert({ C.m_initialState, 'l', A.m_initialState });
			for (int finalState : A.m_finalStates)
			{
				C.m_transitionFunction.insert({ finalState, 'l', newFinalState });
			}
			C.m_transitionFunction.insert({ C.m_initialState, 'l', newFinalState });
			for (int finalState : A.m_finalStates)
			{
				C.m_transitionFunction.insert({ finalState, 'l', A.m_initialState });
			}
			C.m_finalStates.push_back(newFinalState);
			automatonStack.push(C);
		}
	}
	*this = automatonStack.top();
	return automatonStack.top();
}

void NedeterministicFiniteAutomaton::PrintAutomaton() const
{
	std::cout << "\nAutomatul finit nedeterminist corespunzator este:\n\n";
	std::cout << "Starea initiala este: " << m_initialState << std::endl;

	std::cout << "Multimea de stari este: ";
	for (int state : m_states) {
		std::cout << state << " ";
	}
	std::cout << std::endl;

	std::cout << "Alfabetul de intrare este: ";
	for (char symbol : m_inputAlphabet) {
		std::cout << symbol << " ";
	}
	std::cout << std::endl;

	std::cout << "Starile finale sunt: ";
	for (int finalState : m_finalStates) {
		std::cout << finalState << " ";
	}
	std::cout << std::endl;

	std::cout << "Functiile de tranzitie sunt: " << std::endl;
	for (const auto& transition : m_transitionFunction) {
		int currentState = std::get<0>(transition); 
		char symbol = std::get<1>(transition); 
		int nextState = std::get<2>(transition);
		std::cout << "DELTA(" << currentState << ", " << symbol << ") = " << nextState << std::endl;
	}
}