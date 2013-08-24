#include <cstdio>
#include <new>

struct a {
    virtual void f() =0;
};

struct b : public a {
    int m;
    b() : m(0) {}
    virtual void f() { printf("b %d\n", m); }
};

struct c : public a {
    int m;
    c() : m(1) {}
    virtual void f() { printf("C %d\n", m); }
};

int main()
{
    b var;
    c C;
    var.f();
    C.f();

    new((void*)&var) c();
    var.f();

    return 0;
}
