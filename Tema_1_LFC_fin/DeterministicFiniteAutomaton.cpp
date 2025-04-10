#include "DeterministicFiniteAutomaton.h"


std::set<int> DeterministicFiniteAutomaton::LambdaClosure(const NedeterministicFiniteAutomaton& afn, const std::set<int>& states) 
{
    std::set<int> closure = states; 
    std::stack<int> toProcess;

    for (int state : states)
    {
        toProcess.push(state);
    }

    while (!toProcess.empty()) 
    {
        int current = toProcess.top();
        toProcess.pop();

        
        for (const auto& transition : afn.m_transitionFunction) 
        {
            int fromState, toState;
            char input;
            std::tie(fromState, input, toState) = transition;

            if (fromState == current && input == 'l') 
            {

                if (closure.find(toState) == closure.end()) 
                {
                    closure.insert(toState);
                    toProcess.push(toState); 
                }
            }
        }
    }
    return closure;
}


std::set<int> DeterministicFiniteAutomaton::Delta(const NedeterministicFiniteAutomaton& afn, const std::set<int>& currentStates, char symbol)
{
    std::set<int> result;

    for (int state : currentStates) 
    {
        for (const auto& transition : afn.m_transitionFunction) 
        {
            int fromState, toState;
            char input;
            std::tie(fromState, input, toState) = transition;

            if (fromState == state && input == symbol) 
            {
                result.insert(toState);
            }
        }
    }

    return LambdaClosure(afn, result);
}


DeterministicFiniteAutomaton DeterministicFiniteAutomaton::ConvertIntoAFD(const std::string & expression) 
{
    DeterministicFiniteAutomaton dfa;
    NedeterministicFiniteAutomaton afn;
    afn.BuildAutomaton(expression);
    dfa.m_inputAlphabetAFD = afn.m_inputAlphabet;

    std::map<std::set<int>, int> stateMapping;
    std::queue<std::set<int>> toProcess;

    std::set<int> initialState = LambdaClosure(afn, { afn.m_initialState });
    toProcess.push(initialState);
    stateMapping[initialState] = 0;

    dfa.m_statesAFD.push_back(0);
    dfa.m_initialStateAFD = 0;

    int stateCounter = 1;

    while (!toProcess.empty()) 
    {
        std::set<int> currentStates = toProcess.front();
        toProcess.pop();

        for (char symbol : afn.m_inputAlphabet) 
        {
            std::set<int> newState = Delta(afn, currentStates, symbol);

            if (!newState.empty()) {
                if (stateMapping.find(newState) == stateMapping.end()) 
                {
                    stateMapping[newState] = stateCounter++;
                    dfa.m_statesAFD.push_back(stateMapping[newState]);
                    toProcess.push(newState);
                }

                dfa.m_transitionFunctionAFD.insert(
                    std::make_tuple(stateMapping[currentStates], symbol, stateMapping[newState])
                );
            }
        }
    }

    for (const auto& pair : stateMapping) 
    {
        const auto& stateSet = pair.first; 
        const int stateID = pair.second;   
        
        bool isFinal = false;
        for (int subState : stateSet)
        {
            if (std::find(afn.m_finalStates.begin(), afn.m_finalStates.end(), subState) != afn.m_finalStates.end()) 
            {
                isFinal = true;
                break;
            }
        }
        if (isFinal) 
        {
            dfa.m_finalStatesAFD.push_back(stateID);
            
        }
    }


    return dfa;
}

bool DeterministicFiniteAutomaton::CheckWord(const std::string& word) const 
{
    int currentState = m_initialStateAFD;

    for (char symbol : word)
    {
        bool transitionFound = false;

        for (const auto& transition : m_transitionFunctionAFD) 
        {
            if (std::get<0>(transition) == currentState && std::get<1>(transition) == symbol)
            {
                currentState = std::get<2>(transition);
                transitionFound = true;
                break;
            }
        }

        if (!transitionFound) 
        {
            return false;
        }
    }

    return std::find(m_finalStatesAFD.begin(), m_finalStatesAFD.end(), currentState) != m_finalStatesAFD.end();
}

void DeterministicFiniteAutomaton::WriteToFile(const std::string& fileName)
{
    std::ofstream outFile(fileName);
    if (!outFile)
    {
        std::cerr << "Eroare la deschiderea fisierului: " << fileName << std::endl;
        return;
    }
    outFile << "Automatul finit determinist:\n";
    outFile << "Stare initiala: " << m_initialStateAFD << "\n";

    outFile << "Stari: ";
    for (int state : m_statesAFD)
    {
        outFile << state << " ";
    }
    outFile << "\n";

    outFile << "Alfabet: ";
    for (char symbol : m_inputAlphabetAFD)
    {
        outFile << symbol << " ";
    }
    outFile << "\n";

    outFile << "Stari finale: ";
    for (int finalState : m_finalStatesAFD)
    {
        outFile << finalState << " ";
    }
    outFile << "\n";

    outFile << "Tranzitii:\n";
    for (const auto& transition : m_transitionFunctionAFD)
    {
        outFile << "DELTA(" << std::get<0>(transition) << ", " << std::get<1>(transition) << ") = " << std::get<2>(transition) << "\n";
    }

}

void DeterministicFiniteAutomaton::PrintAutomaton() const 
{
    std::cout << "Automatul finit determinist:\n";
    std::cout << "Stare initiala: " << m_initialStateAFD << "\n";

    std::cout << "Stari: ";
    for (int state : m_statesAFD) 
    {
        std::cout << state << " ";
    }
    std::cout << "\n";

    std::cout << "Alfabet: ";
    for (char symbol : m_inputAlphabetAFD) 
    {
        std::cout << symbol << " ";
    }
    std::cout << "\n";

    std::cout << "Stari finale: ";
    for (int finalState : m_finalStatesAFD)
    {
        std::cout << finalState << " ";
    }
    std::cout << "\n";

    std::cout << "Tranzitii:\n";
    for (const auto& transition : m_transitionFunctionAFD)
    {
        std::cout << "DELTA(" << std::get<0>(transition) << ", " << std::get<1>(transition)<< ") = " << std::get<2>(transition) << "\n";
    }
}
bool DeterministicFiniteAutomaton::VerifyAutomaton() const
{
    if (m_initialStateAFD < 0 || std::find(m_statesAFD.begin(), m_statesAFD.end(), m_initialStateAFD) == m_statesAFD.end())
    {
        std::cout << "Automatul nu are o stare initiala valabila!" << std::endl;
        return false;
    }

    if (m_finalStatesAFD.empty())
    {
        std::cout << "Automatul nu are stari finale!" << std::endl;
        return false;
    }

    for (int state : m_statesAFD)
    {
        for (char symbol : m_inputAlphabetAFD)
        {
            bool transitionFound = false;
            for (const auto& transition : m_transitionFunctionAFD)
            {
                if (std::get<0>(transition) == state && std::get<1>(transition) == symbol)
                {
                    transitionFound = true;
                    break;
                }
            }
        }
    }
    std::cout << "Automat Valid!\n";
    return true;
}