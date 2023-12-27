#include "LR1/Grammar.h"
#include <fstream>
#include <set>
#include <stack>

using std::cin;
using std::cout;
using std::ifstream;
using std::stack;

Grammar::Grammar()
{
	S.clear();
	productions.clear();
	Vn.clear();
	Vt.clear();
	first_set.clear();
}

Grammar::Grammar(const Grammar &g)
{
	this->S = g.S;
	this->productions = g.productions;
	this->Vt = g.Vt;
	this->Vn = g.Vn;
	this->first_set = g.first_set;
}

Grammar::~Grammar()
{
	S.clear();
	productions.clear();
	Vn.clear();
	Vt.clear();
	first_set.clear();
}

void Grammar::init()
{
	string start;
	while (true)
	{
		cout << "请输入开始符号：";
		cin >> start;
		if (start < "A" || start > "Z")
		{
			cout << "开始符号应在非终结集中！请重新输入\n";
		}
		else
		{
			S = start;
			break;
		}
	}

	cout << "\n请输入产生式：(输入0结束)\n";
	cout << "形如:S-aA，空串以$表示\n\n";

	while (true)
	{
		string input;
		cin >> input;

		if (input == "0")
			break;

		pstring new_p(input);
		if (new_p.get_valid())
		{
			if (!insert(new_p))
				cout << "产生式重复！\n\n";
			else
				cout << "产生式添加成功！\n\n";
		}
		else
		{
			cout << "产生式添加失败！\n\n";
		}
	}

	del_unreachable_production();

	int i, j;
	for (i = 0; i < productions.size(); i++)
	{
		Vn.insert(productions[i].get_left());
		for (j = 0; j < productions[i].get_right().length(); j++)
		{
			if (inVn(productions[i].get_right()[j]))
			{
				Vn.insert(string(1, productions[i].get_right()[j]));
			}
			else if (inVt(productions[i].get_right()[j]))
			{
				Vt.insert(string(1, productions[i].get_right()[j]));
			}
		}
	}
}

void Grammar::init_from_file(const char *file)
{
	ifstream inFile;
	inFile.open(file, std::ios::in);
	if (!inFile.is_open())
		exit(1);

	string start;
	getline(inFile, start);
	S = start;

	string input;
	while (getline(inFile, input))
	{
		if (input == "0")
			break;

		pstring new_p(input);
		insert(new_p);
	}
	inFile.close();

	del_unreachable_production();

	int i, j;
	for (i = 0; i < productions.size(); i++)
	{
		Vn.insert(productions[i].get_left());
		for (j = 0; j < productions[i].get_right().length(); j++)
		{
			if (inVn(productions[i].get_right()[j]))
			{
				Vn.insert(string(1, productions[i].get_right()[j]));
			}
			else if (inVt(productions[i].get_right()[j]))
			{
				Vt.insert(string(1, productions[i].get_right()[j]));
			}
		}
	}
}

void Grammar::extent()
{
	int i, count = 0, flag = 0;
	for (i = 0; i < productions.size(); i++)
	{
		if (productions[i].get_left() == S)
		{
			count++;
			if (productions[i].get_right().length() == 1 && inVn(productions[i].get_right()[0]))
			{
				flag = 1;
			}
		}
	}

	if (count == 1 && flag == 1)
	{
	}
	else
	{
		string new_S = S + "'";
		pstring new_p(new_S + "-" + S);
		this->S = new_S;
		productions.push_back(new_p);
		sort();
	}

	cout << "\n增广文法如下：\n";
	print_productions();
}

void Grammar::del_unreachable_production()
{
	int i, j;
	// 记录能够推导到的非终结符集
	set<string> reachable;
	stack<string> s;
	s.push(this->S);
	while (!s.empty())
	{
		string ch = s.top();
		s.pop();
		reachable.insert(ch);
		for (i = 0; i < productions.size(); i++)
		{
			if (productions[i].get_left() == ch)
			{
				for (j = 0; j < productions[i].get_right().length(); j++)
				{
					string temp(1, productions[i].get_right()[j]);
					if (inVn(temp[0]))
					{
						if (reachable.find(temp) == reachable.end())
						{
							reachable.insert(temp);
							s.push(temp);
						}
					}
				}
			}
		}
	}

	set<int> del_index;
	for (i = 0; i < productions.size(); i++)
	{
		if (reachable.find(productions[i].get_left()) == reachable.end())
			del_index.insert(i);
	}

	// 下标由大到小的删除不可达产生式
	for (auto del_iter = del_index.rbegin(); del_iter != del_index.rend(); del_iter++)
	{
		productions.erase(productions.begin() + *del_iter);
	}

	sort();
}

void Grammar::cal_First()
{
	int i, j;
	int flag = 1;
	int psize;

	set<string> vn;
	for (i = 0; i < productions.size(); i++)
	{
		vn.insert(productions[i].get_left());
	}

	while (flag)
	{
		flag = 0;
		for (auto target = vn.begin(); target != vn.end(); target++)
		{
			psize = first_set[*target].size();

			for (i = 0; i < productions.size(); i++)
			{
				if (productions[i].get_left() == *target)
				{
					if (productions[i].get_right().length() == 0)
						first_set[*target].insert('$');
					else
					{
						for (j = 0; j < productions[i].get_right().length(); j++)
						{
							char ch = productions[i].get_right()[j];
							if (inVt(ch))
							{
								first_set[*target].insert(productions[i].get_right()[j]);
								break;
							}
							else if (inVn(ch))
							{
								string sch = string(1, ch);
								if (first_set[sch].find('$') == first_set[sch].end())
								{
									first_set[*target].insert(first_set[sch].begin(), first_set[sch].end());
									break;
								}
								else
								{
									for (auto it = first_set[sch].begin(); it != first_set[sch].end(); it++)
									{
										if (*it != '$')
											first_set[*target].insert(*it);
									}
								}
							}
						}
					}
				}
			}

			if (psize != first_set[*target].size())
				flag = 1;
		}
	}
}

const vector<pstring> &Grammar::get_productions() const
{
	return productions;
}

const string &Grammar::get_S() const
{
	return S;
}

const map<string, set<char>> &Grammar::get_first_set() const
{
	return first_set;
}

const set<string> &Grammar::get_Vn() const
{
	return Vn;
}

const set<string> &Grammar::get_Vt() const
{
	return Vt;
}

vector<pstring> &Grammar::get_productions()
{
	return productions;
}

string &Grammar::get_S()
{
	return S;
}

map<string, set<char>> &Grammar::get_first_set()
{
	return first_set;
}

set<string> &Grammar::get_Vn()
{
	return Vn;
}

set<string> &Grammar::get_Vt()
{
	return Vt;
}

void Grammar::print_productions()
{
	int i;
	for (i = 0; i < productions.size(); i++)
	{
		productions[i].print();
	}
}

void Grammar::sort()
{
	int i, j;

	// 首先将开始符号置顶
	for (i = 0; i < productions.size(); i++)
	{
		for (j = i + 1; j < productions.size(); j++)
		{
			if (productions[i].get_left() == S)
				break;
			if (productions[j].get_left() == S)
			{
				pstring temp(productions[i]);
				productions[i] = productions[j];
				productions[j] = temp;

				break;
			}
		}
	}

	// 然后按照字符顺序排列剩余产生式
	for (i = 0; i < productions.size(); i++)
	{
		for (j = i + 1; j < productions.size(); j++)
		{
			if (productions[i].get_left() == S)
				break;
			if (productions[i].get_left() > productions[j].get_left())
			{
				pstring temp(productions[i]);
				productions[i] = productions[j];
				productions[j] = temp;
			}
		}
	}

	// 重新赋值编号
	for (i = 0; i < productions.size(); i++)
	{
		productions[i].set_No(i);
	}
}

bool Grammar::insert(const pstring &p)
{
	if (!duplicate(p))
	{
		productions.push_back(p);
		return true;
	}
	return false;
}

bool Grammar::duplicate(const pstring &p)
{
	int i;
	for (i = 0; i < productions.size(); i++)
	{
		if (productions[i] == p)
		{
			return true;
		}
	}
	return false;
}