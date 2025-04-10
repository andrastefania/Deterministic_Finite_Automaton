#include "PostfixForm.h"

#include<iostream>
#include<stack>
#include<string>

PostfixForm::PostfixForm() {};

bool PostfixForm::IsOperator(char c)
{
	return c == '|' || c == '.' || c == '*';
}
int PostfixForm::Priority(char op)
{
	switch (op)
	{
	case'*': return 3;
	case'.': return 2;
	case'|': return 1;
	default: return 0;
	}
}
std::string PostfixForm::Conversion(const std::string& expression)
{
	std::stack<char>stiva;
	std::string postfix_expression;
	for (char element : expression)
	{
		if ((element >= 'a' && element <= 'z') || (element >= 'A' && element <= 'Z') || (element >= '0' && element <= '9'))
			postfix_expression += element;
		else if (element == '(')
			stiva.push(element);
		else if (element == ')')
		{
			while (!stiva.empty() && stiva.top() != '(')
			{
				postfix_expression += stiva.top();
				stiva.pop();
			}
			if (!stiva.empty())
				stiva.pop();
			else
			{
				std::cout << "Error. Incorrect Expresssion";
				return "";
			}
		}
		else if (IsOperator(element))
		{
			while (!stiva.empty() && stiva.top() != '(' && Priority(stiva.top()) >= Priority(element))
			{
				postfix_expression += stiva.top();
				stiva.pop();
			}
			stiva.push(element);
		}
		else
		{
			std::cout << "Error. Invalid Character";
			return "";
		}
	}
	while (!stiva.empty())
	{
		if (stiva.top() == '(')
		{
			std::cout << "Error.";
			return "";
		}
		postfix_expression += stiva.top();
		stiva.pop();
	}
	return postfix_expression;
}

std::vector<char> PostfixForm::TransformToVector(const std::string& expresecsion)
{
	std::vector<char>result;
	for (char c : expresecsion)
		result.push_back(c);
	return result;
}