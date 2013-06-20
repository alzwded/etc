#include <cstdio>
#include "minifloat.h"

int main()
{
    Minifloat one(1);
    Minifloat three(3);
    Minifloat onetwentyfour(1024);

    printf("%d %d %d\n", (int)one, (int)three, (int)onetwentyfour);

    Minifloat o1023(1023);
    Minifloat o64(64);
    Minifloat o48(48);
    Minifloat o65000(65000);
    printf("%d %d %d %d\n", (int)o1023, (int)o64, (int)o48, (int)o65000);

    Minifloat o1025(1025);
    printf("%d\n", (int)o1025);

    return 0;
}
