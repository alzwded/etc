#include <immintrin.h>
#include <cstdlib>
#include <string>
#include <cstdio>
#include <vector>
#include <tuple>
#include <utility>

#ifdef _MSC_VER
inline unsigned msclz(unsigned int u) { unsigned long index = 0; return _BitScanReverse(&index, u), sizeof(unsigned)*8-1-index; }
# define CLZ(U) msclz(U)
inline unsigned int log2(unsigned int u)
{
    unsigned long index = 0;
    return _BitScanReverse(&index, u), index;
}
#elif defined(__GNUC__)
# define CLZ(U) __builtin_clz(U)
inline unsigned int log2(unsigned int u)
{
    return (!u) ? 0 : 31u - __builtin_clz(u);
}
#endif


int main()
{
    printf("Tests:\n");
    #define TEST(i, o) std::tuple<const char*, unsigned, unsigned>{ "log2(" #i ")", i ## u, o ## u }
    std::vector<std::tuple<const char*, unsigned, unsigned>> tests {
        TEST(0, 0),
        TEST(1, 0),
        TEST(2, 1),
        TEST(42, 5),
        TEST(1000000, 19),
        TEST(32, 5),
        TEST(31, 4),
        TEST(33, 5),
    };
    printf("%16s %8s %8s %8s\n", "Test", "Got", "Expected", "Result");
    for(auto&& kv : tests)
    {
        auto got = log2(std::get<1>(kv));
        printf("%16s %8u %8u %8s\n",
            std::get<0>(kv),
            got,
            std::get<2>(kv),
            ( got == std::get<2>(kv) ? "OK" : "FAIL" )
            );
    }
}
