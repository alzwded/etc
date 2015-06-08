// compile with g++ -g --std=gnu++11 testFunction.cpp
//      or with cl /EHsc /Zi /DEBUG testFunction.cpp
// should print 1 2 3 4 5 7

#include <cstddef>
#include <cstdio>

#include <list>
#include <functional>
#include <tuple>

std::list<
    std::tuple<size_t, size_t, std::function<size_t(size_t)>, double const*>
    > g_list;

void add(size_t start, size_t end, std::function<size_t(size_t)> f, double const* ptr)
{
    auto tpl = std::make_tuple(start, end, f, ptr);
    g_list.push_back(tpl);
}

void iterateOver()
{
    for(auto&& i : g_list) {
        auto j = std::get<0>(i);
        auto endj = std::get<1>(i);
        auto fn = std::get<2>(i);
        auto ptr = std::get<3>(i);

        for(; j < endj; j = fn(j)) {
            printf("%f ", ptr[j]);
        }
    }
    printf("\n");
}

int main(int argc, char* argv[])
{
    auto defaultStep = [](size_t i) -> size_t { return i + 1; };
    static double const aarray[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    static size_t const narray = sizeof(aarray)/sizeof(aarray[0]);

    add(0, narray/2, defaultStep, aarray);

    auto doubleStep = [](size_t i) -> size_t { return i + 2; };
    add(narray/2, narray, doubleStep, aarray);

    iterateOver();

    return 0;
}
