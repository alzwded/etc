/*
description: number generator that follows a binary pattern of the form
             [xX*01]* where x, X and * represent DC values and 0 & 1 are
             explicit
author:      Vlad Mesco
date:        Tue May 21 16:44:37 EEST 2013
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HI 1u
#define LO 0u

#ifndef TRUE
# define TRUE 1
#endif

#ifndef FALSE
# define FALSE 0
#endif

#define print_atom(B) do{ \
    if(B.bit) putc('1', stdout); \
    else putc('0', stdout); \
}while(0)

#define print_bin(n, size) do{\
    size_t i = 0; \
    for(; i < size; ++i) { \
        print_atom(n[i]); \
    } \
}while(0)

#define print_dec(n, size, fail) do{ \
    size_t i = 0; \
    unsigned long dec = 0; \
    if(size >= 8u*sizeof(unsigned long)) { *fail = 1; break; } \
    for(; i < size; ++i) { \
        dec <<= 1; \
        if(n[i].bit) dec++; \
    } \
    printf("%d", dec); \
}while(0)

#define print(n, size) do{ \
    unsigned char fail = 0; \
    print_dec(n, size, &fail); if(!fail) printf("\t"); print_bin(n, size); \
}while(0)

typedef struct element_s {
    int bit:1;
    int mutable:1;
} element_t;
typedef element_t* element_a;

unsigned char increment(element_a n, size_t size)
{
    element_a p = &n[size - 1];
    while(p >= n) {
        if(!p->mutable) {
            --p;
            continue;
        } else if(p->bit) {
            p->bit = LO;
            --p;
            continue;
        } else {
            p->bit = HI;
            return TRUE;
        }
    }
    return FALSE;
}

void generate(const char* p)
{
    size_t i = 0;
    size_t size = strlen(p);
    element_a n = (element_a)malloc(sizeof(element_t) * size);
    for(; i < size; ++i) {
        switch(p[i]) {
            case '0':
                n[i].bit = LO;
                n[i].mutable = FALSE;
                break;
            case '1':
                n[i].bit = HI;
                n[i].mutable = FALSE;
                break;
            case 'x':
            case 'X':
            case '*':
                n[i].bit = LO;
                n[i].mutable = TRUE;
                break;
            default:
                fprintf(stderr, "unknown character %c in pattern %s\n", p[i], p);
                exit(2);
        }
    }

    printf("combinations for %s:\n", p);

    while(TRUE) {
        print(n, size);
        printf("\n");
        if(!increment(n, size)) break;
    }

    printf("\n");

    free(n);
}

int main(int argc, char* argv[])
{
    char** p;
    char** last = &argv[argc];
    if(argc < 2) {
        printf("usage: %s pattern1 pattern2 ...\n", argv[0]);
        return 1;
    }

    for(p = &argv[1]; p != last; ++p) {
        generate(*p);
    }

    return 0;
}
