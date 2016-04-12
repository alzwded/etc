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

A libA;

void fill(B* b)
{
    b->data_ = (int*)b->alloc_(sizeof(int) * 8);
    for(size_t i = 0; i < 8; ++i) b->data_[i] = i;
}
