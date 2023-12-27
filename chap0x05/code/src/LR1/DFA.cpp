#include "LR1/DFA.h"
#include <set>

using std::cout;

DFA::DFA()
{
	node_set.clear();
	edge_set.clear();
}

DFA::DFA(Grammar &g)
{
	G = g;
	Node temp;
	int i;
	auto &g_productions = g.get_productions();
	for (i = 0; i < g_productions.size(); i++)
	{
		if (g_productions[i].get_left() == g.get_S())
		{
			temp.insert(g_productions[i]);
			temp.set_divide_index(1);
			break;
		}
	}
	temp.get_closure(G);
	add_node(temp, i);
}

DFA::~DFA()
{
	node_set.clear();
	edge_set.clear();
}

void DFA::cal_dfa()
{
	int i, j;
	int flag = 1;
	int last_time = 0; // 记录上一次求到那个节点，减少循环次数
	while (flag)
	{
		flag = 0;

		int psize = node_set.size();
		for (i = last_time; i < psize; i++)
		{
			set<string> trans;
			Node n = node_set[i];

			// 获取所有“可以走”的字符（即·后面的字符）
			auto &n_productions = n.get_productions();
			for (j = 0; j < n_productions.size(); j++)
			{
				int dot_pos = n_productions[j].get_dot();
				if (dot_pos < n_productions[j].get_right().length())
				{
					trans.insert(string(1, n_productions[j].get_right()[dot_pos]));
				}
			}

			// 开始走字符构造新节点
			for (auto it = trans.begin(); it != trans.end(); it++)
			{
				Node new_node;
				for (j = 0; j < n_productions.size(); j++)
				{
					int dot_pos = n_productions[j].get_dot();
					if (dot_pos < n_productions[j].get_right().length())
					{
						if (string(1, n_productions[j].get_right()[dot_pos]) == *it)
						{
							pstring newp = n_productions[j];
							newp.shift_dot();
							new_node.insert(newp);
						}
					}
				}
				int pnum = new_node.get_productions().size();
				new_node.set_divide_index(pnum);
				new_node.get_closure(G);

				int modify_index;
				add_node(new_node, modify_index);
				new_node.set_number(modify_index);

				Edge new_edge;
				new_edge.set_Start(n);
				new_edge.set_End(new_node);
				new_edge.set_trans(*it);
				if (add_edge(new_edge))
				{
					flag = 1;
				}
			}

			if (flag == 1)
			{
				last_time = i + 1;
				break;
			}
		}
	}
}

int DFA::find_node(const Node &n)
{
	int i, j, k;
	int count;
	for (i = node_set.size() - 1; i >= 0; i--)
	{
		count = 0;
		if (node_set[i].get_productions().size() == n.get_productions().size())
		{
			for (j = 0; j < node_set[i].get_divide_index(); j++)
			{
				for (k = 0; k < n.get_divide_index(); k++)
				{
					if (node_set[i].get_productions()[j] == n.get_productions()[k])
					{
						count++;
						break;
					}
				}
			}

			if (count == n.get_divide_index())
				return i;
		}
	}
	return -1;
}

bool DFA::add_node(const Node &n, int &index)
{
	if (!duplicate_node(n, index))
	{
		node_set.push_back(n);
		for (int i = 0; i < node_set.size(); i++)
		{
			node_set[i].set_number(i);
		}

		index = node_set.size() - 1;
		return true;
	}
	return false;
}

bool DFA::add_edge(const Edge &e)
{
	if (!duplicate_edge(e))
	{
		edge_set.push_back(e);
		return true;
	}
	return false;
}

bool DFA::duplicate_node(const Node &n, int &index)
{
	int i, j, k;
	int count;
	auto &n_productions = n.get_productions();

	for (i = 0; i < node_set.size(); i++)
	{
		count = 0;
		auto &node_productions = node_set[i].get_productions();
		if (node_productions.size() == n_productions.size())
		{
			for (j = 0; j < node_set[i].get_divide_index(); j++)
			{
				for (k = 0; k < n.get_divide_index(); k++)
				{
					if (node_productions[j] == n_productions[k])
					{
						count++;
						break;
					}
				}
			}

			if (count == n.get_divide_index())
			{
				index = i;
				return true;
			}
		}
	}
	index = -1;
	return false;
}

bool DFA::duplicate_edge(const Edge &e)
{
	int i;
	for (i = 0; i < edge_set.size(); i++)
	{
		if (edge_set[i].get_Start().get_number() == e.get_Start().get_number())
		{
			if (edge_set[i].get_End().get_number() == e.get_End().get_number())
			{
				if (edge_set[i].get_trans() == e.get_trans())
				{
					return true;
				}
			}
		}
	}
	return false;
}

Grammar &DFA::get_G()
{
	return G;
}

vector<Node> &DFA::get_node_set()
{
	return node_set;
}

vector<Edge> &DFA::get_edge_set()
{
	return edge_set;
}

void DFA::print_info() const
{
	cout << "DFA的信息如下：\n";
	cout << "共有节点" << node_set.size() << "个，边" << edge_set.size() << "条\n\n";

	cout << "节点详情如下：\n\n";
	int i, j;
	for (i = 0; i < node_set.size(); i++)
	{
		cout << "节点标号：I" << node_set[i].get_number() << "\n\n";
		cout << "编号\t产生式\t展望字符\n";
		auto &node_productions = node_set[i].get_productions();
		for (j = 0; j < node_productions.size(); j++)
		{
			pstring temp = node_productions[j];
			string right = temp.get_right();
			right.insert(temp.get_dot(), 1, '.');
			temp.set_right(right);

			cout << j << "\t" << temp << "\t" << temp.get_prospection() << '\n';
		}
		cout << '\n';
	}

	cout << "边集详情如下：\n\n";
	for (i = 0; i < edge_set.size(); i++)
	{
		cout << "第" << i + 1 << "条边：\n";
		cout << "I" << edge_set[i].get_Start().get_number() << " -" << edge_set[i].get_trans() << "-> "
			 << "I" << edge_set[i].get_End().get_number() << "\n\n";
	}
}