#pragma once
#include "DFA.h"
#include "Lexer/Lex.h"
#include <map>
#include <vector>

class Table
{
public:
	Table();
	Table(const DFA &);
	void init_table();				// 根据DFA中的节点信息，先初始化空的table
	void cal_table();				// 遍历DFA，为table赋值
	void print_table();				// 输出表格
	void Move();					// 遍历边集，赋值移入和转移
	void Reduce();					// 遍历点集，赋值规约
	bool predict(string &s);		// 预测函数，字符串
	bool predict(vector<symbol> &); // 预测函数，symbol集合，该函数带有语义分析功能
	~Table();

private:
	vector<map<string, string>> table;
	DFA graph;
};
