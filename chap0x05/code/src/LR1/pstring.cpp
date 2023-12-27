#include "LR1/pstring.h"
#include <unordered_set>
using std::unordered_set;

static int pnum = 0; // 产生式标号

// 非终结符
static const unordered_set<char> Vn{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

// 终结符
static const unordered_set<char> Vt{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '+', '-', '*', '/', '=', '(', ')', '#', '$', '%', '^', '&', '|', '!'};

bool inVt(char c)
{
	return Vt.find(c) != Vt.end();
}

bool inVn(char c)
{
	return Vn.find(c) != Vn.end();
}

pstring::pstring()
{
	left.clear();
	right.clear();
	dot = 0;
	No = pnum++;
	valid = true;
	prospection = '#';
}

pstring::pstring(const string &input)
{
	if (legal(input))
	{
		left = input.substr(0, input.find('-'));
		right = input.substr(input.find('-') + 1, input.length());
		if (right == "$")
			right.clear();
		valid = true;
	}
	else
	{
		left.clear();
		right.clear();
		valid = false;
	}

	this->dot = 0;
	this->prospection = '#';
	if (valid)
		this->No = pnum++;
}

pstring::pstring(const pstring &p)
{
	this->dot = p.dot;
	this->left = p.left;
	this->right = p.right;
	this->No = p.No;
	this->valid = p.valid;
	this->prospection = p.prospection;
}

pstring::~pstring()
{
	left.clear();
	right.clear();
}

bool pstring::legal(const string &input)
{
	if (!inVn(input[0]))
	{
		std::cerr << "无效的产生式，左端应为非终结符！\n";
		return false;
	}
	else
	{
		string right;
		int i = input.find("-");
		if (i == string::npos)
		{
			std::cout << "产生式格式有误!\n";
			return false;
		}
		else
		{
			right = input.substr(i + 1, input.length());
			for (i = 0; i < right.length(); i++)
			{
				if (!inVt(right[i]) && !inVn(right[i]))
				{
					std::cout << "产生式右部含非法字符！\n";
					return false;
				}
			}
		}
	}
	return true;
}

void pstring::print()
{
	std::cout << this->left << "->" << this->right << "\n\n";
}

bool pstring::shift_dot()
{
	if (dot < this->right.length())
	{
		dot++;
		return true;
	}
	return false;
}

void pstring::set_left(const string &left)
{
	this->left = left;
}

void pstring::set_right(const string &right)
{
	this->right = right;
}

void pstring::set_prospection(const string &c)
{
	prospection = c;
}

void pstring::set_No(int num)
{
	No = num;
}

string &pstring::get_left()
{
	return left;
}

string &pstring::get_right()
{
	return right;
}

bool pstring::get_valid()
{
	return valid;
}

int pstring::get_No()
{
	return No;
}

int pstring::get_dot()
{
	return dot;
}

string &pstring::get_prospection()
{
	return prospection;
}

bool operator==(const pstring &p1, const pstring &p2)
{
	if (p1.left == p2.left && p1.right == p2.right)
		if (p1.prospection == p2.prospection)
			if (p1.dot == p2.dot)
				return true;
	return false;
}

ostream &operator<<(ostream &out, const pstring &p)
{
	out << p.left << "->" << p.right;
	return out;
}