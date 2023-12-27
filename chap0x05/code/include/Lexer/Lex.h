#pragma once
#include <vector>
#include <string>

using std::string;
using std::vector;

struct symbol // 定义一个结构体，用于语义分析
{
	string ch = "a";
	double val = 0.0;
	bool operand = false; // true表示运算数，false表示运算符
};

vector<symbol> parsing(const string &);