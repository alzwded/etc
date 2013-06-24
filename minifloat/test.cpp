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

#define cTEST(i, WHAT) do{ \
    ofN++; \
    int got = (WHAT); \
    failed += !got; \
    printf("%6d. (%c) roundtrip of %7d = %7d\n", ofN - currentofN, (!got ? 'x' : 'v'), i, (int)Minifloat(i)); \
}while(0)

#define bTEST(STR, WHAT) do{ \
    ofN++; \
    int got = (WHAT); \
    failed += !got; \
    printf("%3d. (%c) " STR "\n", ofN - currentofN, (!got ? 'x' : 'v') ); \
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
    Minifloat inf(Minifloat::INF);
    Minifloat ninf(Minifloat::NEG_INF);
    Minifloat nan(Minifloat::NAN);
    Minifloat o7(7);
    Minifloat o8(8);
    Minifloat on2(-2);
    bTEST("7 < 8", o7 < o8);
    bTEST("8 < 7", !(o8 < o7));
    bTEST("-2 < 7", on2 < o7);
    bTEST("8 == 8", o8 == o8);
    bTEST("8 >= 8", o8 >= o8);
    bTEST("nan == 8", !(nan == o8));
    bTEST("nan == nan", !(nan == nan));
    bTEST("8 >= ninf", o8 >= ninf);
    bTEST("8 <= ninf", !(o8 <= ninf));
    iTEST("1 / 0 = ", (int)(Minifloat(1) / Minifloat(0)), MF_INF_INT);
    iTEST("-1 / 0 = ", (int)(Minifloat(-1) / Minifloat(0)), MF_NEG_INF_INT);
    iTEST("1 / -0 = ", (int)(Minifloat(1) / Minifloat(Minifloat::NEG_ZERO)), -MF_INF_INT);
    iTEST("-1 / -0 = ", (int)(Minifloat(-1) / Minifloat(Minifloat::NEG_ZERO)), MF_INF_INT);
    o8 = Minifloat::NEG_INF;
    iTEST("neginf assignation: ", (int)(o8), MF_NEG_INF_INT);
    o8 = 8;
    iTEST("int 8 assignation: ", (int)(o8), 8);
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
    bTEST("anything + nan = nan", (o8 + nan).IsNaN());
    bTEST("inf + anything = inf", (inf + o8).IsInf());
    bTEST("inf - inf = nan", (inf - inf).IsNaN());
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
    iTEST("-2 + (-2) = ", (int)(Minifloat(-2) + Minifloat(-2)), -4);
    iTEST("-2 - 2 = ", (int)(Minifloat(-2) - Minifloat(2)), -4);
TESTEND(addition)

TESTSTART(substraction)
    iTEST("36 - 9 = ", (int)(Minifloat(36) - Minifloat(9)), 26);
    iTEST("18 - 5 = ", (int)(Minifloat(18) - Minifloat(5)), 13);
    iTEST("-9 + 36 = ", (int)(Minifloat(-9) + Minifloat(36)), 26);
    iTEST("128 - 104 = ", (int)(Minifloat(128) - Minifloat(104)), 24);
    /* test to see if extra 1 bit set for normal numbers is still there
       after substraction */
    iTEST("-124 + 128 = ", (int)(Minifloat(-124) + Minifloat(128)), 4);
    iTEST("-112 + 128 = ", (int)(Minifloat(-112) + Minifloat(128)), 16);
    iTEST("128 - 96 = ", (int)(Minifloat(128) - Minifloat(96)), 32);
    iTEST("128 - 88 = ", (int)(Minifloat(128) - Minifloat(88)), 40);
    iTEST("128 - 80 = ", (int)(Minifloat(128) - Minifloat(80)), 48);
    iTEST("128 - 10 = ", (int)(Minifloat(128) - Minifloat(10)), 112);
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
    iTEST("0 * 3 = ", (int)(Minifloat(0) * Minifloat(3)), 0);
    bTEST("0 * inf = nan", (Minifloat(0) * Minifloat(Minifloat::INF)).IsNaN());
    bTEST("4096 * -9999 = -inf", (Minifloat(4096) * Minifloat(-9999)).IsNegInf());
    bTEST("4096 * 2048 = inf", (Minifloat(4096) * Minifloat(2048)).IsInf());
    iTEST("20 * 40 = ", (int)(Minifloat(20) * Minifloat(40)), 768);
    iTEST("10 * 10 = ", (int)(Minifloat(10) * Minifloat(10)), 96);
    iTEST("2 * 3 = ", (int)(Minifloat(2) * Minifloat(3)), 6);
    iTEST("8 * 8 = ", (int)(Minifloat(8) * Minifloat(8)), 64);
    iTEST("2 * 20 = ", (int)(Minifloat(2) * Minifloat(20)), 40);
    iTEST("2 * 40 = ", (int)(Minifloat(2) * Minifloat(40)), 80);
    iTEST("2 * 60 = ", (int)(Minifloat(2) * Minifloat(60)), 120);
    iTEST("2 * 80 = ", (int)(Minifloat(2) * Minifloat(80)), 160);
    iTEST("3 * 20 = ", (int)(Minifloat(3) * Minifloat(20)), 60);
    iTEST("4 * 20 = ", (int)(Minifloat(4) * Minifloat(20)), 80);
    iTEST("13 * 13 = ", (int)(Minifloat(13) * Minifloat(13)), 160);
    iTEST("14 * 14 = ", (int)(Minifloat(14) * Minifloat(14)), 192);
    iTEST("9 * 20 = ", (int)(Minifloat(9) * Minifloat(20)), 176);
    iTEST("9 * 40 = ", (int)(Minifloat(9) * Minifloat(40)), 352);
    iTEST("32 * 128 = ", (int)(Minifloat(32) * Minifloat(128)), 4096);
TESTEND(multiplication)

TESTSTART(division)
    printf("60 / 20 = %d\n", (int)(Minifloat(60) / Minifloat(20)));
    printf("6 / 2 = %d\n", (int)(Minifloat(6) / Minifloat(2)));
TESTEND(division)
    
TESTSTART(conversion) 
    for(int i = -122880; i <= 122880; ++i) {
        Minifloat m(i);
        cTEST(i, (int)(Minifloat(i)) == (int)(Minifloat((int)Minifloat(i))));
    }

    printf("printing entire range of values: \n");
    printf("idx. hex. decimal\n");
    printf(" -1. %02Xh. #nan\n", Minifloat(Minifloat::NAN).raw().raw);
    printf("  0. %02Xh.-#inf\n", Minifloat(Minifloat::NEG_INF).raw().raw);
    int idx = 1;
    for(Minifloat i(-122880); !i.IsInf(); ++i) {
        printf("%3d. %02Xh.%7d\n", idx++, i.raw().raw, (int)i);
    }
    printf("255. %02Xh. #inf\n", Minifloat(Minifloat::NEG_INF).raw().raw);
    printf("\n");

    Minifloat f(Minifloat::NAN);
    for(idx = 0; idx <= 0xFF; ++idx) {
        printf("for hex %02Xh, f = ", idx);
        f.raw().raw = idx;
        // TODO implement << and >> ostreamistream operators
        if(f.IsNaN()) printf(" #nan\n");
        else if(f.IsNegInf()) printf("-#inf\n");
        else if(f.IsInf()) printf(" #inf\n");
        else if(f.raw().raw == 0x80) printf("     -0\n");
        else printf("%7d\n", (int)f);
    }
TESTEND(conversion)

int main(int argc, char* argv[])
{
    TEST(substraction);
    TEST(meta);
    TEST(addition);
    TEST(multiplication);
    TEST(division);

    if(argc > 1) TEST(conversion);

    EPILOGUE();

    return (failed != 0);
}
