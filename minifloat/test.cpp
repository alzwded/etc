#include <cstdio>
#include "minifloat.h"

static size_t currentfailed = 0;
static size_t currentofN = 0;
static size_t failed = 0;
static size_t ofN = 0;

#define TCAT(STR) do{ \
    printf("CATEGORY: " STR ":\n"); \
    printf("-----------------------------------------------------------------------\n"); \
}while(0)

#define iTEST(STR, WHAT, EXPECTED_VALUE) do{ \
    ofN++; \
    int got = (WHAT); \
    int f = (got != EXPECTED_VALUE); \
    failed += f; \
    printf("%3d. (%c) " STR "%d (expecting %d)\n", ofN - currentofN, (f ? 'x' : 'v'), got, (EXPECTED_VALUE)); \
}while(0)

#define TESTSTART(name) \
void name () \
{ \
    TCAT(#name); \
    currentfailed = failed; \
    currentofN = ofN; \

#define TESTEND(name) \
    printf("-----------------------------------------------------------------------\n"); \
    printf("category:" #name " failed: %d of %d\n", failed - currentfailed, ofN - currentofN); \
    printf("-----------------------------------------------------------------------\n"); \
}

#define EPILOGUE() \
    printf("-----------------------------------------------------------------------\n"); \
    printf("failed: %d of %d\n", failed, ofN);


#define TEST(name) name();

TESTSTART(meta)
    Minifloat o8(8);
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
TESTEND(meta)

TESTSTART(addition)
    iTEST("1 + 2 = ", (int)(Minifloat(1) + Minifloat(2)), 3);
    iTEST("1 + 7 = ", (int)(Minifloat(1) + Minifloat(7)), 8);
    iTEST("15 + 1 = ", (int)(Minifloat(15) + Minifloat(1)), 16);
    iTEST("130 + 32 = ", (int)(Minifloat(130) + Minifloat(32)), 160);
    iTEST("32 + 130 = ", (int)(Minifloat(32) + Minifloat(130)), 160);
    iTEST("130 + 36 = ", (int)(Minifloat(130) + Minifloat(36)), 160);
    iTEST("36 + 130 = ", (int)(Minifloat(36) + Minifloat(130)), 160);
    iTEST("130 + 10 = ", (int)(Minifloat(130) + Minifloat(10)), 128);
    iTEST("10 + 130 = ", (int)(Minifloat(10) + Minifloat(130)), 128);
    iTEST("130 + 20 = ", (int)(Minifloat(130) + Minifloat(20)), 144); // exp 5, sig 1
    iTEST("20 + 130 = ", (int)(Minifloat(20) + Minifloat(130)), 144); // exp 4 (why???), sig 1
    Minifloat o7(7);
    Minifloat o8(8);
    iTEST("8 + 7 = ", (int)(o8 + o7), 15);
    iTEST("7 + 8 = ", (int)(o7 + o8), 15);
    iTEST("12 + 7 = ", (int)(Minifloat(12) + Minifloat(7)), 18);
    iTEST("9 + 20 = ", (int)(Minifloat(9) + Minifloat(20)), 28);
    iTEST("20 + 9 = ", (int)(Minifloat(20) + Minifloat(9)), 28);
    iTEST("10 + 36 = ", (int)(Minifloat(10) + Minifloat(36)), 44);
    iTEST("36 + 10 = ", (int)(Minifloat(36) + Minifloat(10)), 44);
    iTEST("10 + 65 = ", (int)(Minifloat(10) + Minifloat(65)), 72);
    iTEST("65 + 10 = ", (int)(Minifloat(65) + Minifloat(10)), 72);
TESTEND(addition)

TESTSTART(substraction)
    iTEST("36 - 9 = ", (int)(Minifloat(36) - Minifloat(9)), 26);
    iTEST("18 - 5 = ", (int)(Minifloat(18) - Minifloat(5)), 13);
    iTEST("-9 + 36 = ", (int)(Minifloat(-9) + Minifloat(36)), 26);
    /* test to see if extra 1 bit set for normal numbers is still there
       after substraction */
    iTEST("-124 + 128 = ", (int)(Minifloat(-124) + Minifloat(128)), 4);
    iTEST("-112 + 128 = ", (int)(Minifloat(-112) + Minifloat(128)), 16);
    iTEST("-128 + 10 = ", (int)(Minifloat(-128) + Minifloat(10)), -112);
    iTEST("-128 + 128 = ", (int)(Minifloat(-128) + Minifloat(128)), 0);
    iTEST("10 - 128 = ", (int)(Minifloat(10) - Minifloat(128)), -112);
    iTEST("1 - 3 = ", (int)(Minifloat(1) - Minifloat(3)), -2);
    iTEST("1 - 18 = ", (int)(Minifloat(1) - Minifloat(18)), -16);
    iTEST("1 - 20 = ", (int)(Minifloat(1) - Minifloat(20)), -18);
TESTEND(substraction)

TESTSTART(multiplication)
    // TODO to be determined what this should result, right now it
    //    depends on int-conversion and _real_ hard-core minifloat
    //    math might cause round-off errors beyond those caused by the
    //    conversion. Will see
    printf("20 * 40 = %d\n", (int)(Minifloat(20) * Minifloat(40)));
TESTEND(multiplication)

TESTSTART(division)
    printf("60 / 20 = %d\n", (int)(Minifloat(60) / Minifloat(20)));
    printf("6 / 2 = %d\n", (int)(Minifloat(6) / Minifloat(2)));
TESTEND(division)
    
TESTSTART(conversion) 
    for(int i = -122880; i <= 122880; ++i) {
        Minifloat m(i);
        printf("int %7d minifloat %7d\n", i, (int)m);
    }
TESTEND(conversion)

int main()
{
    TEST(substraction);
    TEST(meta);
    TEST(addition);
    TEST(multiplication);
    TEST(division);

    TEST(conversion);

    EPILOGUE();

    return (failed != 0);
}
