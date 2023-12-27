#pragma once

#include "Grammar.h"

class Node : public Grammar
{
public:
	Node();
	Node(const Node &);
	Node(const Grammar &);
	~Node();
	void set_divide_index(int);
	void set_number(int);
	void get_closure(Grammar &);
	string get_First(const string &, Grammar &);

	int get_number() const;
	int get_divide_index() const;

protected:
	int number;		  // 节点编号
	int divide_index; // 原始推导式和闭包的分界处
};
