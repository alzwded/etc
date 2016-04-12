#include <stdlib.h>
#include <head.h>
#include <stdio.h>

int data[8];

void* myAlloc(size_t size)
{
    printf("myAlloc called %lu %p\n", size, data);
    return data;
}

void myFree(void* ptr)
{
    printf("myFree called %p %p\n", ptr, data);
}

int main()
{
    extern void fill(B*);

    printf("should call malloc/free and myAlloc/myFree\n");

    {
        A a1, a2(myAlloc, myFree);
        fill(&a1);
        fill(&a2);
#define SHOW(X)\
        for(size_t i = 0; i < 8; ++i) {\
            printf("%d ", (X).data_[i]);\
        }\
        printf("\n");
        SHOW(a1);
        SHOW(a2);
    }
    printf("\n");


    printf("should call wrong malloc/wrong free\n");
    extern A libA;
    fill(&libA);
}
