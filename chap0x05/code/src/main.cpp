#include "LR1/Table.h"
#include <ctime>

using std::cout;

int main()
{
	clock_t begin = clock();
	Grammar g;
	g.init_from_file("calculator_grammar.txt");
	g.extent();
	g.cal_First();

	DFA dfa(g);
	dfa.cal_dfa();

	Table predict(dfa);
	predict.cal_table();
	predict.print_table();

	string input = "2+(1+2.5)^2*2=";

	vector<symbol> parse_result = parsing(input);

	if (predict.predict(parse_result))
	{
		cout << "\n计算完毕！\n\n";
	}
	else
	{
		cout << "\n输入非法！\n\n";
	}

	cout << "总共用时：" << (double)(clock() - begin) / CLOCKS_PER_SEC << '\n';
	return 0;
}