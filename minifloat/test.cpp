#include <cstdio>
#include "minifloat.h"

int main()
{
    Minifloat inf(Minifloat::INF);
    Minifloat ninf(Minifloat::NEG_INF);
    Minifloat nan(Minifloat::NAN);
    Minifloat ref(1);
    Minifloat inf2(122881 << 3);
    Minifloat ninf2(-122881 << 3);
    printf("inf: %d\n", inf.IsInfinity());
    printf("neginf: %d\n", ninf.IsInfinity());
    printf("nan: %d\n", nan.IsNaN());
    printf("ref not inf: %d\n", !ref.IsInfinity());
    printf("ref not nan: %d\n", !ref.IsNaN());
    printf("inf from conversion: %d\n", inf2.IsInfinity());
    printf("neginf from conversion: %d\n", ninf2.IsInfinity());
    for(int i = -122880; i <= 122880; ++i) {
        Minifloat m(i);
        printf("int %7d minifloat %7d\n", i, (int)m);
    }

    return 0;

    /*Minifloat one(1);
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
    */

    return 0;
}
