#pragma once
#include "pstring.h"
#include <vector>
#include <set>
#include <unordered_set>
#include <map>

using std::map;
using std::set;
using std::vector;

class Grammar
{
public:
	Grammar();
	Grammar(const Grammar &);
	~Grammar();

	void init();						   // 初始化函数，将依次调用：删除不可达式、增广文法、排序
	void init_from_file(const char *file); // 读入文件初始化
	void extent();						   // 增广文法
	void sort();						   // 将产生式排序（开始符号置顶，按字符从小到大）
	void del_unreachable_production();	   // 删除不可达式
	void cal_First();					   // 计算First集合

	bool insert(const pstring &);	 // 插入新的产生式
	bool duplicate(const pstring &); // 检查是否有重复的产生式
	void print_productions();

	const vector<pstring> &get_productions() const;
	const string &get_S() const;
	const map<string, set<char>> &get_first_set() const;
	const set<string> &get_Vn() const;
	const set<string> &get_Vt() const;

	vector<pstring> &get_productions();
	string &get_S();
	map<string, set<char>> &get_first_set();
	set<string> &get_Vn();
	set<string> &get_Vt();

protected:
	vector<pstring> productions; // 产生式集合
	string S;					 // 开始字符
	set<string> Vn;
	set<string> Vt;
	map<string, set<char>> first_set; // First集合
};
