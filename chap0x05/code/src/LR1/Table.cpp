#include "LR1/Table.h"
#include <iomanip>
#include <stack>

using std::cin;
using std::cout;
using std::stack;

Table::Table()
{
	table.clear();
}

Table::Table(const DFA &dfa)
{
	graph = dfa;
	init_table();
}

void Table::init_table()
{
	int i;

	auto &Vt = graph.get_G().get_Vt();
	auto &Vn = graph.get_G().get_Vn();

	for (i = 0; i < graph.get_node_set().size(); i++)
	{
		map<string, string> new_map;

		for (auto it = Vt.begin(); it != Vt.end(); it++)
		{
			new_map[*it] = "";
		}

		new_map["#"] = "";

		for (auto it = Vn.begin(); it != Vn.end(); it++)
		{
			new_map[*it] = "";
		}

		table.push_back(new_map);
	}
}

void Table::cal_table()
{
	Move();
	Reduce();
}

void Table::print_table()
{
	vector<string> keys;
	auto &Vt = graph.get_G().get_Vt();
	auto &Vn = graph.get_G().get_Vn();

	for (auto it = Vt.begin(); it != Vt.end(); it++)
	{
		if (*it != "#")
			keys.push_back(*it);
	}
	keys.push_back("#");

	for (auto it = Vn.begin(); it != Vn.end(); it++)
	{
		keys.push_back(*it);
	}

	cout << "-------------------------预测分析表-------------------------\n\n";
	for (auto it = keys.begin(); it != keys.end(); it++)
	{
		cout << '\t' << *it;
	}
	cout << '\n';

	int i, j;
	for (i = 0; i < table.size(); i++)
	{
		cout << i;
		for (j = 0; j < keys.size(); j++)
		{
			cout << "\t" << table[i][keys[j]];
		}
		cout << '\n';
	}

	cout << "\n-----------------规约产生式-----------------\n\n";
	for (i = 0; i < graph.get_G().get_productions().size(); i++)
	{
		cout << i << ". " << graph.get_G().get_productions()[i] << '\n';
	}
}

void Table::Move()
{
	auto &edge_set = graph.get_edge_set();

	for (auto edge_iter = edge_set.begin(); edge_iter != edge_set.end(); edge_iter++)
	{
		int start = (*edge_iter).get_Start().get_number();
		int end = (*edge_iter).get_End().get_number();
		string &trans = (*edge_iter).get_trans();

		if (inVt(trans[0]))
		{
			if (table[start][trans] != "")
			{
				cout << "不是LR(1)文法，错误!\n";
				exit(1);
			}
			table[start][trans] = "S" + std::to_string(end);
		}
		else
		{
			if (table[start][trans] != "")
			{
				cout << "不是LR(1)文法，错误!\n";
				exit(1);
			}
			table[start][trans] = std::to_string(end);
		}
	}
}

void Table::Reduce()
{
	auto &node_set = graph.get_node_set();
	auto &productions = graph.get_G().get_productions();

	for (auto node_iter = node_set.begin(); node_iter != node_set.end(); node_iter++)
	{
		int node_index = (*node_iter).get_number();

		auto &node_productions = (*node_iter).get_productions();
		for (auto it1 = node_productions.begin(); it1 != node_productions.end(); it1++)
		{
			if ((*it1).get_dot() == (*it1).get_right().length()) // ·在最后需要规约
			{
				int i = 0;
				for (auto it2 = productions.begin(); it2 != productions.end(); it2++, i++) // 遍历原始产生式，找到规约项
				{
					if ((*it1).get_left() == (*it2).get_left() && (*it1).get_right() == (*it2).get_right())
					{
						if (i != 0)
						{
							if (table[node_index][(*it1).get_prospection()] != "")
							{
								cout << "不是LR(1)文法，错误!\n";
								exit(1);
							}
							table[node_index][(*it1).get_prospection()] = "r" + std::to_string(i);
						}
						else
						{
							if (table[node_index][(*it1).get_prospection()] != "")
							{
								cout << "不是LR(1)文法，错误!\n";
								exit(1);
							}
							table[node_index][(*it1).get_prospection()] = "acc";
						}
						break;
					}
				}
			}
		}
	}
}

bool Table::predict(string &s)
{
	if (s == "")
	{
		cout << "请输入句子：\n";
		cin >> s;
	}
	if (s[s.length() - 1] != '#')
		s += "#";

	stack<string> SYMBOL; // 符号栈
	stack<int> STATE;	  // 状态栈

	SYMBOL.push("#");
	STATE.push(0);

	cout << "-------------------语法分析-------------------\n\n";
	cout << "步骤\t状态栈\t\t符号栈\t\t输入串\t\t操作\n";
	int i = 0;
	int count = 0;
	string current;
	while (true)
	{
		int state = STATE.top();
		string &symbol = SYMBOL.top();
		current = string(1, s[i]);

		if (inVn(symbol[0])) // Goto
		{
			STATE.push(stoi(table[state][symbol]));
			state = STATE.top();
		}

		// 步骤
		cout << count++ << "\t";

		// 状态栈
		stack<int> temp_state_stack = STATE;
		vector<int> temp_state;
		while (!temp_state_stack.empty())
		{
			temp_state.push_back(temp_state_stack.top());
			temp_state_stack.pop();
		}
		for (int j = temp_state.size() - 1; j >= 0; j--)
		{
			cout << temp_state[j] << " ";
		}
		cout << "\t\t";

		// 符号栈
		stack<string> temp_symbol_stack = SYMBOL;
		vector<string> temp_symbol;
		while (!temp_symbol_stack.empty())
		{
			temp_symbol.push_back(temp_symbol_stack.top());
			temp_symbol_stack.pop();
		}
		for (int j = temp_symbol.size() - 1; j >= 0; j--)
		{
			cout << temp_symbol[j];
		}
		cout << "\t\t";

		// 输入串
		cout << s.substr(i) << "\t\t";

		// 操作，分类输出
		string action;
		action = table[state][current];

		if (action == "acc")
		{
			cout << "分析成功\n\n";
			return true;
		}

		if (action == "")
		{
			cout << "无法匹配，错误！\n\n";
			return false;
		}

		if (action[0] == 'S') // 移入
		{
			int push_in = stoi(action.substr(1));
			STATE.push(push_in);
			SYMBOL.push(current);
			i++;
			cout << "移入" << current << '\n';
		}
		else if (action[0] == 'r')
		{
			int index = stoi(action.substr(1));
			for (int j = graph.get_G().get_productions()[index].get_right().length() - 1; j >= 0; j--)
			{
				if (SYMBOL.top()[0] == graph.get_G().get_productions()[index].get_right()[j])
				{
					SYMBOL.pop();
					STATE.pop();
				}
				else
					return false;
			}
			cout << "规约：" << graph.get_G().get_productions()[index] << '\n';
			SYMBOL.push(graph.get_G().get_productions()[index].get_left());
		}
	}

	return false;
}

bool Table::predict(vector<symbol> &parse_result)
{
	int i;
	if (parse_result[parse_result.size() - 1].ch != "#")
	{
		parse_result.push_back(symbol{"#", 0, false});
	}

	stack<symbol> SYMBOL;	// 符号栈
	stack<int> STATE;		// 状态栈
	stack<string> SEMANTIC; // 语义栈（存结果）
	auto &g_productions = graph.get_G().get_productions();

	SYMBOL.push(symbol{"#", 0, false});
	STATE.push(0);

	cout << "------------------------------------------语法分析------------------------------------------\n\n";
	cout << "步骤 \t状态栈 \t\t\t符号栈 \t\t语义栈 \t\t\t输入串 \t\t\t\t操作\n";
	i = 0;
	int count = 0;
	symbol current; // 当前读头
	while (true)
	{
		int out_length = 0; // 这是用来控制输出串对齐的
		int state = STATE.top();
		symbol topsym = SYMBOL.top();
		current = parse_result[i];

		if (inVn(topsym.ch[0])) // Goto
		{
			STATE.push(stoi(table[state][topsym.ch]));
			state = STATE.top();
		}

#pragma region 输出分析过程

		// 步骤
		cout << count++ << " \t";

		// 状态栈
		stack<int> temp_state_stack = STATE;
		vector<int> temp_state;
		while (!temp_state_stack.empty())
		{
			temp_state.push_back(temp_state_stack.top());
			temp_state_stack.pop();
		}
		for (int j = temp_state.size() - 1; j >= 0; j--)
		{
			cout << temp_state[j] << " ";
			out_length += std::to_string(temp_state[j]).length() + 1;
		}
		if (out_length <= 7)
			cout << "\t\t\t";
		else if (out_length / 8 == 1)
		{
			cout << "\t\t";
		}
		else if (out_length / 8 == 2)
		{
			cout << "\t";
		}

		// 符号栈
		stack<symbol> temp_symbol_stack = SYMBOL;
		vector<string> temp_symbol;
		while (!temp_symbol_stack.empty())
		{
			temp_symbol.push_back(temp_symbol_stack.top().ch);
			temp_symbol_stack.pop();
		}
		for (int j = temp_symbol.size() - 1; j >= 0; j--)
		{
			cout << temp_symbol[j];
		}
		cout << "\t\t";

		// 语义栈
		stack<string> temp_semantic_stack = SEMANTIC;
		vector<string> temp_semantic;
		while (!temp_semantic_stack.empty())
		{
			temp_semantic.push_back(temp_semantic_stack.top());
			temp_semantic_stack.pop();
		}
		for (int j = temp_semantic.size() - 1; j >= 0; j--)
		{
			if (temp_semantic[j] == "+" || temp_semantic[j] == "-" || temp_semantic[j] == "*" || temp_semantic[j] == "/" || temp_semantic[j] == "(" || temp_semantic[j] == ")" || temp_semantic[j] == "^")
			{
				cout << temp_semantic[j];
			}
			else
			{
				cout << stod(temp_semantic[j]);
			}
		}
		cout << "\t\t\t";

		// 输入串
		out_length = 0;
		for (int j = i; j < parse_result.size(); j++)
		{
			cout << parse_result[j].ch;
			out_length += 1;
		}
		if (out_length <= 7)
			cout << "\t\t\t\t";
		else
			cout << "\t\t\t";

#pragma endregion

		// 操作，分类输出
		string action;
		action = table[state][current.ch];

		if (action == "acc")
		{
			cout << "分析成功\n";
			cout << "\n运算结果为：" << stod(SEMANTIC.top()) << "\n\n";
			return true;
		}

		if (action == "")
		{
			cout << "无法匹配，错误！\n\n";
			return false;
		}

		if (action[0] == 'S') // 移入
		{
			int push_in = stoi(action.substr(1));
			STATE.push(push_in);
			SYMBOL.push(current);
			if (current.operand)
			{
				SEMANTIC.push(std::to_string(current.val));
			}
			else
			{
				SEMANTIC.push(current.ch);
			}

			i++;
			cout << "移入" << current.ch << '\n';
		}
		else if (action[0] == 'r')
		{
			int index = stoi(action.substr(1));
			vector<symbol> reduce;

			for (int j = g_productions[index].get_right().length() - 1; j >= 0; j--)
			{
				if (SYMBOL.top().ch[0] == g_productions[index].get_right()[j])
				{
					if (SYMBOL.top().operand)
						reduce.push_back(SYMBOL.top());
					SYMBOL.pop();
					STATE.pop();
					SEMANTIC.pop();
				}
				else
				{
					cout << "无法匹配，错误！\n\n";
					return false;
				}
			}
			symbol reduce_result{g_productions[index].get_left(), 0, true};

			switch (index) // 根据规约哪一个产生式决定语义
			{
			case 0:
				break;
			case 1:
				reduce_result.val = reduce[1].val - reduce[0].val;
				break;
			case 2:
				reduce_result.val = reduce[0].val;
				break;
			case 3:
				reduce_result.val = reduce[0].val + reduce[1].val;
				break;
			case 4:
				reduce_result.val = reduce[0].val;
				break;
			case 5:
				reduce_result.val = pow(reduce[1].val, reduce[0].val);
				break;
			case 6:
				reduce_result.val = reduce[0].val;
				break;
			case 7:
				reduce_result.val = reduce[0].val;
				break;
			case 8:
				reduce_result.val = reduce[0].val;
				break;
			case 9:
				reduce_result.val = reduce[0].val * reduce[1].val;
				break;
			case 10:
				reduce_result.val = reduce[1].val / reduce[0].val;
				break;
			default:
				cout << "无法规约！\n";
				return false;
			}

			cout << "规约：" << g_productions[index] << '\n';
			SYMBOL.push(reduce_result);
			SEMANTIC.push(std::to_string(reduce_result.val));
		}
	}

	return false;
}

Table::~Table()
{
	table.clear();
}