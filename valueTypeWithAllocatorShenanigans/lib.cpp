#include <stdio.h>

static void* malloc(size_t)
{
    static int dontcrash[8];
    printf("wrong malloc called\n");
    return dontcrash;
}

static void free(void*)
{
    printf("wrong free called\n");
}

#include <head.h>

#if 0
// you get an error here on C++03 that malloc and free need to have external
// linkage (as per standard), but this is library internal code anyway and
// we can compile the library however we want
A_t libA;
#endif

extern "C" void fill(B* b)
{
    b->data_ = (int*)b->alloc_(sizeof(int) * 8);
    for(size_t i = 0; i < 8; ++i) b->data_[i] = i;
}
