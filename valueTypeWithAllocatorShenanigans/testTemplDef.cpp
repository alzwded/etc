// author:  Vlad Mesco
// date:    12/04/2016
// description:
//   Create a by-value type that requires an allocator for internal
//   allocations that uses the calling library's malloc, or any
//   allocator it might want to use via arguments
//
//   compile with g++ --std=gnu++11 testTemplDef.cpp

#include <cstdio>
#include <cstdlib>

typedef void* (*alloc_fn)(size_t);
typedef void (*free_fn)(void*);

struct A {
    int* x;
    alloc_fn afn;
    free_fn ffn;
};

template<alloc_fn ALLOCFN = malloc, free_fn DEALLOCFN = free>
struct B : public A
{
    B()
    {
        afn = ALLOCFN;
        ffn = DEALLOCFN;
    }
    ~B()
    {
        ffn(x);
    }
};

void f(A* a)
{
    a->x = (int*)a->afn(5);
    for(size_t i = 0; i < 5; a->x[i] = i++);
}

typedef B<> Bd;

void* myalloc(size_t)
{
    static int buf[5];
    printf("my alloc called\n");
    return &buf;
}

void myfree(void*)
{
    printf("my free called\n");
}

int main()
{
    Bd a1;
    B<myalloc, myfree> a2;
    f(&a1);
    for(size_t i = 0; i < 5; ++i) {
        printf("%d\n", a1.x[i]);
    }
    f(&a2);
    for(size_t i = 0; i < 5; ++i) {
        printf("%d\n", a2.x[i]);
    }
}
