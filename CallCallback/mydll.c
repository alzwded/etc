#include <stdio.h>
int f(int a, int b)
{
    return a*100 + b;
}

void printint(int v)
{
    printf(__FILE__ ": %d\n", v);
}
