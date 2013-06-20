#include <cstdio>
#include "minifloat.h"

int main()
{
    Minifloat o7(7);
    Minifloat o8(8);
    printf("7 + 8 = %d\n", (int)(o8 + o7));
    printf("12 + 7 = %d\n", (int)(Minifloat(12) + Minifloat(7)));
    printf("18 - 5 = %d\n", (int)(Minifloat(18) - Minifloat(5)));
    printf("36 - 9 = %d\n", (int)(Minifloat(36) - Minifloat(9)));
    printf("1 - 3 = %d\n", (int)(Minifloat(1) - Minifloat(3)));
    printf("20 * 40 = %d\n", (int)(Minifloat(20) * Minifloat(40)));
    printf("60 / 20 = %d\n", (int)(Minifloat(60) / Minifloat(20)));
    printf("6 / 2 = %d\n", (int)(Minifloat(6) / Minifloat(2)));
    printf("1 / 0 = %d\n", (int)(Minifloat(1) / Minifloat(0)));
    printf("-1 / 0 = %d\n", (int)(Minifloat(-1) / Minifloat(0)));
    printf("1 / -0 = %d\n", (int)(Minifloat(1) / Minifloat(Minifloat::NEG_ZERO)));
    printf("-1 / -0 = %d\n", (int)(Minifloat(-1) / Minifloat(Minifloat::NEG_ZERO)));
    o8 = Minifloat::NEG_INF;
    printf("neginf assignation %d\n", (int)(o8));
    o8 = 8;
    printf("int 8 assignation %d\n", (int)(o8));
    Minifloat inf(Minifloat::INF);
    Minifloat ninf(Minifloat::NEG_INF);
    Minifloat nan(Minifloat::NAN);
    Minifloat ref(1);
    Minifloat inf2(122881 << 3);
    Minifloat ninf2(-122881 << 3);
    printf("inf: %d\n", inf.IsInfinity() && inf.IsInf());
    printf("neginf: %d\n", ninf.IsInfinity() && ninf.IsNegInf());
    printf("nan: %d\n", nan.IsNaN());
    printf("ref not inf: %d\n", !ref.IsInfinity());
    printf("ref not nan: %d\n", !ref.IsNaN());
    printf("inf from conversion: %d\n", inf2.IsInfinity() && inf2.IsInf());
    printf("neginf from conversion: %d\n", ninf2.IsInfinity() && ninf2.IsNegInf());
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
