#include "LR1/Node.h"
#include <set>
#include <stack>

static int node_num = 0;

Node::Node()
{
	this->number = node_num++;
	this->productions.clear();
	this->divide_index = 0;
	this->S.clear();
}

Node::Node(const Node &n)
{
	this->productions = n.productions;
	this->divide_index = n.divide_index;
	this->number = n.number;
	this->S = n.S;
}

Node::Node(const Grammar &g)
{
	number = node_num++;
	divide_index = g.get_productions().size();
	productions = g.get_productions();
	S = g.get_S();
	first_set = g.get_first_set();
}

Node::~Node()
{
	this->productions.clear();
}

void Node::set_divide_index(int index)
{
	divide_index = index;
}

void Node::set_number(int index)
{
	number = index;
}

void Node::get_closure(Grammar &g)
{
	int i, j, k;
	int flag = 1;
	int pindex = 0;
	auto &g_productions = g.get_productions();
	while (flag)
	{
		flag = 0;
		int psize = productions.size();

		for (i = pindex; i < psize; i++)
		{
			int dot_pos = productions[i].get_dot();
			if (dot_pos < productions[i].get_right().length())
			{
				char ch = productions[i].get_right()[dot_pos];
				string bch = productions[i].get_prospection();

				if (dot_pos + 1 < productions[i].get_right().length())
					bch = productions[i].get_right().substr(dot_pos + 1) + productions[i].get_prospection();

				if (inVn(ch))
				{
					string prospections = get_First(bch, g);
					for (j = 0; j < g_productions.size(); j++)
					{
						if (g_productions[j].get_left() == string(1, ch))
						{
							for (k = 0; k < prospections.length(); k++)
							{
								string prospection = string(1, prospections[k]);
								pstring newp = g_productions[j];
								newp.set_prospection(prospection);
								insert(newp);
							}
						}
					}
				}
			}
		}
		pindex = psize;

		if (productions.size() != psize)
			flag = 1;
	}
}

string Node::get_First(const string &p, Grammar &g)
{
	string first;
	int hasEmpty;
	auto &gfirst = g.get_first_set();

	for (char ch : p)
	{
		hasEmpty = 0;
		if (inVt(ch))
		{
			first += ch;
			break;
		}
		else
		{
			string sch = string(1, ch);
			for (auto it = gfirst[sch].begin(); it != gfirst[sch].end(); it++)
			{
				if (*it != '$')
					first += *it;
				else
					hasEmpty = 1;
			}

			if (hasEmpty == 0)
				break;
		}
	}

	return first;
}

int Node::get_number() const
{
	return number;
}

int Node::get_divide_index() const
{
	return divide_index;
}