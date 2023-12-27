#include "LR1/Table.h"
#include <ctime>

using std::cout;

int main()
{
    // 记录程序开始执行的时间
    clock_t begin = clock();

    // 创建文法对象
    Grammar g;
    // 从文件中读取文法规则
    g.init_from_file("calculator_grammar.txt");
    // 对文法进行扩展和计算First集
    g.extent();
    g.cal_First();

    // 构建DFA
    DFA dfa(g);
    dfa.cal_dfa();

    // 构建预测分析表
    Table predict(dfa);
    predict.cal_table();
    // 打印预测分析表
    predict.print_table();

    // 待分析的输入字符串
    string input = "2%1=";

    // 进行语法分析
    vector<symbol> parse_result = parsing(input);

    // 使用预测分析表进行分析
    if (predict.predict(parse_result))
    {
        cout << "\n计算完毕！\n\n";
    }
    else
    {
        cout << "\n输入非法！\n\n";
    }

    // 打印程序执行时间
    cout << "总共用时：" << (double)(clock() - begin) / CLOCKS_PER_SEC << '\n';
    return 0;
}
