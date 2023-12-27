#pragma once

#include "Node.h"

class Edge
{
public:
	Edge() {}
	Edge(const Edge &);
	~Edge() {}

	const Node &get_Start() const;
	const Node &get_End() const;
	const string &get_trans() const;

	Node &get_Start();
	Node &get_End();
	string &get_trans();

	void set_Start(const Node &);
	void set_End(const Node &);
	void set_trans(const string &);

private:
	Node Start;
	Node End;
	string trans;
};
