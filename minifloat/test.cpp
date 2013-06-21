#include <cstdio>
#include "minifloat.h"

static size_t failed = 0;
static size_t ofN = 0;

#define iTEST(STR, WHAT, EXPECTED_VALUE) do{ \
    ofN++; \
    int got = (WHAT); \
    int f = (got != EXPECTED_VALUE); \
    failed += f; \
    printf("(%c) " STR "%d (expecting %d)\n", (f ? 'x' : 'v'), got, (EXPECTED_VALUE)); \
}while(0)

int main()
{
    Minifloat o7(7);
    Minifloat o8(8);
    iTEST("8 + 7 = ", (int)(o8 + o7), 15);
    iTEST("7 + 8 = ", (int)(o7 + o8), 15);
    iTEST("12 + 7 = ", (int)(Minifloat(12) + Minifloat(7)), 18);
    iTEST("9 + 20 = ", (int)(Minifloat(9) + Minifloat(20)), 28);
    iTEST("20 + 9 = ", (int)(Minifloat(20) + Minifloat(9)), 28);
    iTEST("10 + 36 = ", (int)(Minifloat(10) + Minifloat(36)), 44);
    iTEST("36 + 10 = ", (int)(Minifloat(36) + Minifloat(10)), 44);
    iTEST("18 - 5 = ", (int)(Minifloat(18) - Minifloat(5)), 13);
    iTEST("36 - 9 = ", (int)(Minifloat(36) - Minifloat(9)), 26);
    iTEST("1 - 3 = ", (int)(Minifloat(1) - Minifloat(3)), -2);
    // TODO to be determined what this should result, right now it
    //    depends on int-conversion and _real_ hard-core minifloat
    //    math might cause round-off errors beyond those caused by the
    //    conversion. Will see
    printf("20 * 40 = %d\n", (int)(Minifloat(20) * Minifloat(40)));
    printf("60 / 20 = %d\n", (int)(Minifloat(60) / Minifloat(20)));
    printf("6 / 2 = %d\n", (int)(Minifloat(6) / Minifloat(2)));
    iTEST("1 / 0 = ", (int)(Minifloat(1) / Minifloat(0)), MF_INF_INT);
    iTEST("-1 / 0 = ", (int)(Minifloat(-1) / Minifloat(0)), -MF_INF_INT);
    iTEST("1 / -0 = ", (int)(Minifloat(1) / Minifloat(Minifloat::NEG_ZERO)), -MF_INF_INT);
    iTEST("-1 / -0 = ", (int)(Minifloat(-1) / Minifloat(Minifloat::NEG_ZERO)), MF_INF_INT);
    o8 = Minifloat::NEG_INF;
    iTEST("neginf assignation: ", (int)(o8), -MF_INF_INT);
    o8 = 8;
    iTEST("int 8 assignation: ", (int)(o8), 8);
    Minifloat inf(Minifloat::INF);
    Minifloat ninf(Minifloat::NEG_INF);
    Minifloat nan(Minifloat::NAN);
    Minifloat ref(1);
    Minifloat inf2(122881 << 3);
    Minifloat ninf2(-122881 << 3);
    iTEST("inf: ", inf.IsInfinity() && inf.IsInf(), 1);
    iTEST("neginf: ", ninf.IsInfinity() && ninf.IsNegInf(), 1);
    iTEST("nan: ", nan.IsNaN(), 1);
    iTEST("ref not inf: ", !ref.IsInfinity(), 1);
    iTEST("ref not nan: ", !ref.IsNaN(), 1);
    iTEST("inf from conversion: ", inf2.IsInfinity() && inf2.IsInf(), 1);
    iTEST("neginf from conversion: ", ninf2.IsInfinity() && ninf2.IsNegInf(), 1);
    // TODO
    for(int i = -122880; i <= 122880; ++i) {
        Minifloat m(i);
        printf("int %7d minifloat %7d\n", i, (int)m);
    }

    printf("-----------------------------------------------------------------------\n");
    printf("failed: %d of %d\n", failed, ofN);

    return (failed != 0);
}
