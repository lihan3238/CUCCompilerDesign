#pragma once
#include <iostream>
#include <string>

using std::ostream;
using std::string;

class pstring
{
	friend bool operator==(const pstring &p1, const pstring &p2); // 等于重载
	friend ostream &operator<<(ostream &out, const pstring &p);	  // 输出重载
public:
	pstring();
	pstring(const string &);
	pstring(const pstring &);
	~pstring();
	bool legal(const string &);
	void print();
	bool shift_dot();
	void set_left(const string &);
	void set_right(const string &);
	void set_prospection(const string &);
	void set_No(int);
	string &get_left();
	string &get_right();
	bool get_valid();
	int get_No();
	int get_dot();
	string &get_prospection();

protected:
	string left;
	string right;
	int dot;			// ·所在下标
	int No;				// 产生式编号
	bool valid;			// 产生式合法标志
	string prospection; // 展望字符
};

bool inVn(char);

bool inVt(char);
