#include <stdio.h>

int failed = 0;
#define TEST(X) do{ \
    printf("%-50s%10s\n", #X, ((X) ? "OK" : (failed++, "FAILED"))); \
}while(0)

int main(int argc, char* argv[])
{
    TEST(1);
    TEST(0);
    TEST(1==1==1==1==1==1==1==1==1==1==1==1==1==1==1==1==1);
    TEST(1 && 1 || 0);
    TEST((1 && 1) ^ 1);

    printf("failed %d\n", failed);

    return 0;
}
