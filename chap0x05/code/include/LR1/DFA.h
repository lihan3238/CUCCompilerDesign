#pragma once

#include "Edge.h"

class DFA
{
public:
	DFA();
	DFA(Grammar &);
	~DFA();
	void cal_dfa();
	int find_node(const Node &);		// 已经【弃用】的方法。返回节点在节点集中的下标
	bool add_node(const Node &, int &); // 添加新节点，先检查是否已有，若已有则返回下标；反之，则加入，并返回下标
	bool add_edge(const Edge &);
	bool duplicate_node(const Node &, int &); // 检查节点集合中是否已有Node，若有则返回其下标，反之返回-1
	bool duplicate_edge(const Edge &);		  // 检查边集中是否已有Edge

	Grammar &get_G();
	vector<Node> &get_node_set();
	vector<Edge> &get_edge_set();

	void print_info() const;

private:
	vector<Node> node_set;
	vector<Edge> edge_set;
	Grammar G;
};
