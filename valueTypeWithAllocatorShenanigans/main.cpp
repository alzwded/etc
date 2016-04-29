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

extern "C" void fill(B*);

int main()
{
    printf("should call malloc/free and myAlloc/myFree\n");

    {
        A_t a1;
        A<myAlloc, myFree> a2;
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


#if 0
    printf("expect wrong free call\n");
    extern B libA;
    fill(&libA);
#endif
}
