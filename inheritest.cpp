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
    printf("on auto variables\n");
    b var;
    c C;
    var.f();
    C.f();

    new((void*)&var) c();
    var.f();

    printf("on pointers\n");
    a* ptr1 = new b();
    ptr1->f();
    new( (void*)ptr1 ) c();
    ptr1->f();

    return 0;
}
