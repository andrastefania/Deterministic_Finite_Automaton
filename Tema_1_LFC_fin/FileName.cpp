#include <iostream>
#include <vector>
#include "NedeterministicFiniteAutomaton.h"
#include "DeterministicFiniteAutomaton.h"
#include "PostfixForm.h"
#include <fstream>

int main()
{
	std::string input ;
	std::ifstream read("inputFile.txt");
	read >> input;
	
	std::string rezPostfixForm;
	PostfixForm exp;
	rezPostfixForm = exp.Conversion(input);
	if (rezPostfixForm != "")
	{
		NedeterministicFiniteAutomaton afn;
		afn.BuildAutomaton(input);
		DeterministicFiniteAutomaton afd;
		afd = afd.ConvertIntoAFD(input);
		afd.VerifyAutomaton();
		int option;
		std::cout << "Alegeti o optiune:\n\n";
		std::cout << "1.Afisarea expresiei regulate si a formei postfixata corespunzatoare.\n";
		std::cout << "2.Afisarea automatului nedeterminist.\n";
		std::cout << "3.Afisarea automatului determinist.\n";
		std::cout << "4.Verificarea daca un cuvant este acceptat.\n";
		std::cout << "5.Iesire din program.\n";
		do
		{
			std::cin >> option;
			switch (option)
			{
			case 1:
				std::cout << "Expresia regulata citita este: " << input << "\n";
				std::cout << "Forma Poloneza Postfixata este: " << rezPostfixForm << "\n";
				break;
			case 2:
				afn.PrintAutomaton();
				break;
			case 3:
				
				afd.WriteToFile("outputFile.txt");
				afd.PrintAutomaton();
				break;
			case 4:
			{
				std::string word;
				std::cout << "Introduceti cuvantul: ";
				std::cin >> word;
				if (afd.CheckWord(word) == true)
					std::cout << "Cuvantul este acceptat de catre automat.\n";
				else
					std::cout << "Cuvantul nu este acceptat de catre automat.\n";
				break;
			}
			case 5:
				return 0;
			default:
				std::cout << "Optiune Invalida.\n";
				break;
			}

		} while (option);
	}
	return 0;
}