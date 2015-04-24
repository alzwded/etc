// compile with g++ --std=gnu++14

#include "AtExitDo.hpp"
#include <fstream>
#include <cstdio>
#include <algorithm>


void test1()
{
    printf("test1: acquiring some resource\n");
    int x = 5, y = 7;
    // VS wants auto&& here
    auto test_atExit = AtExit::Do([](int& x, int& y) {
        printf("test1: releasing resource %d+%d\n", x, y);
    }, x, y);
    printf("test1: doing stuff\n");
    printf("test1: returning\n");
    return;
}

void test2()
{
    printf("test2: acquiring data\n");
    int* a = new int[32];
    auto test2_atExit = AtExit::Do([&]() {
        printf("test2: deleting a\n");
        delete[] a;
    });
    printf("test2: filling a with 0s\n");
    std::fill(a, a + 32, 0);
    printf("test2: a = { ");
    std::for_each(a, a + 32, [](int& a) { printf("%d ", a); });
    printf("\n");
    printf("test2: returning\n");
    return;
}

void test3()
{
    struct a { static void f(int x, std::string s, std::string const& ss) { printf("%s: releasing %d for %s\n", s.c_str(), x, ss.c_str()); } };

    printf("test3: acquiring data\n");
    std::string s("test3");
    std::string const& ss(s);
    auto test3_atExit = AtExit::Do(&a::f, 3, s, ss);
    printf("test3: processing\n");
    printf("test3: returning\n");
    return;
}

int main()
{
    test1();
    printf("%s\n", std::string(62, '-').c_str());
    test2();
    printf("%s\n", std::string(62, '-').c_str());
    test3();
    return 0;
}
