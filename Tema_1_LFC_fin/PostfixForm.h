#pragma once
#include<string>
#include<vector>
class PostfixForm
{
public:
	PostfixForm();
	bool IsOperator(char c);
	int Priority(char op);

	std::string Conversion(const std::string& expression);
	std::vector<char> TransformToVector(const std::string& expression);

private:
	std::string m_expression;
	std::vector<char>m_result;
};