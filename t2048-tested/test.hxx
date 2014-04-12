#ifndef TEST_HXX
#define TEST_HXX

#include <cassert>
#include <cstring>

/*
   My nice little test bed header.

Usage:
    // define a test bundle
    TEST_BUNDLE_BEGIN(BundleName)
    {
        TEST_BEGIN(TestingAssertTrueDoesntFail) {
            assert(true);
        } TEST_END(TestingAssertTrueDoesntFail)

        TEST_BEGIN(TestingAnIntegerEqualsItself) {
            int anInteger = 42;
            assert(anInteger == anInteger);
            assert(anInteger == 42);
        } TEST_END(TestingAnIntegerEqualsItself)

        TEST_BEGIN(FailWorks) {
            return 255;
        } TEST_END(FailWorks)
    }
    TEST_BUNDLE_END(BundleName);

    // run the bundle
    int main(int argc, char* argv)
    {
        int testResult(0);
        TEST_MAIN(BundleName, testResult, argc, argv);
        return testResult;
    }
*/

#define TEST_NELEMENTS(A) ( sizeof(A) / sizeof(A[0]) )

#define TEST_BUNDLE_BEGIN(BUNDLE_NAME) \
static struct { \
    const char* name; \
    int (*fn)(); \
} BUNDLE_NAME[] =

#define TEST_BEGIN(NAME) \
{ #NAME, [&]() { \
    printf("%s: ", #NAME);

#define TEST_END(NAME) \
    return 0; \
               }},

#define TEST_BUNDLE_END(BUNDLE_NAME)

#define TEST_MAIN(BUNDLE_NAME, ret, argc, argv) \
do { \
    size_t i = 0; \
    assert(argc == 2); \
    if(argv[1][0] == '-' && argv[1][1] == 'l') { \
        for(; i < TEST_NELEMENTS(BUNDLE_NAME); ++i) { \
            printf("%s ", BUNDLE_NAME[i].name); \
        } \
        printf("\n"); \
        ret = 0; \
        break; \
    } \
    for(; i < TEST_NELEMENTS(BUNDLE_NAME); ++i) { \
        if(strcmp(argv[1], BUNDLE_NAME[i].name) == 0) { \
            int retCode = BUNDLE_NAME[i].fn(); \
            if(retCode == 0) printf("OK\n"); \
            else printf("Fail\n"); \
            return retCode; \
        } \
    } \
} while(0)

#endif
