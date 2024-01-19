#include <stdio.h>
#include <stdlib.h>
int main()
{

    int x = 1;
    for (int i = 0; i < 10; i++)
    {
        x = x + 1;
    }
    printf("return:%d", x);
    system("pause");
    return 0;
}