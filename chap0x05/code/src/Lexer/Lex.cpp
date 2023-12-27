#include "Lexer/Lex.h"
#include <iostream>

using std::cout;

vector<symbol> parsing(const string &input)
{
	vector<symbol> result;

	cout << "-----------------------------词法分析-----------------------------\n\n";
	cout << "\t\t记号名\t属性值\n";

	int i, j;
	for (i = 0; i < input.length();)
	{
		symbol new_operand, new_operator;

		int value;
		for (j = i; j < input.length(); j++)
		{
			if (input[j] == '+' || input[j] == '-' || input[j] == '*' || input[j] == '/' || input[j] == '(' || input[j] == ')' || input[j] == '^' || input[j] == '=')
			{
				new_operator.ch = string(1, input[j]);
				break;
			}
		}

		if (j > 0)
		{
			if (input[j - 1] != ')')
			{
				if (input[j] != '(')
				{
					new_operand.val = stod(input.substr(i, j - i));
					new_operand.operand = true;

					result.push_back(new_operand);
					result.push_back(new_operator);

					cout << "\t\t< 操作数，" << input.substr(i, j - i) << " >\n";
				}
				else
				{
					result.push_back(new_operator);
				}
			}
			else
			{
				result.push_back(new_operator);
			}
		}
		else
		{
			if (new_operator.ch == "(")
				result.push_back(new_operator);
			else
			{
				cout << "输入不合法！\n";
				return result;
			}
		}

		cout << "\t\t< 运算符，" << input[j] << " >\n";

		if (input[j] == '=')
			break;

		i = j + 1;
	}
	cout << '\n';

	if (result[result.size() - 1].ch == "=")
	{
		result.erase(result.begin() + result.size() - 1);
	}

	return result;
}