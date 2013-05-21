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
#include <inttypes.h>

#define HI 1u
#define LO 0u

#ifndef TRUE
# define TRUE 1
#endif

#ifndef FALSE
# define FALSE 0
#endif

#ifndef BOOL
# define BOOL unsigned char
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

#define print_dec(N, SIZE, VALID, ODEC) do{ \
    size_t i = 0; \
    unsigned long DEC = 0; \
    if(SIZE >= 8u*sizeof(unsigned long)) { *VALID = FALSE; break; } \
    else *VALID = TRUE; \
    for(; i < SIZE; ++i) { \
        DEC <<= 1; \
        if(n[i].bit) DEC++; \
    } \
    printf("%lu", DEC); \
    *ODEC = DEC; \
}while(0)

#define print(N, SIZE, DEC, VALIDdEC) do{ \
    print_dec(N, SIZE, VALIDdEC, DEC); if(*VALIDdEC) printf("\t"); print_bin(N, SIZE); \
}while(0)

void printHex(int64_t* hex, size_t size, const char* s)
{
    char sizeSeal[3] = "";
    // 6 .. 16 5 .. 8 4 .. 1 3 .. 1 2 .. 1 0 .. 1
    size_t sizeInBytes = (size > 3) ? 2 << (size - 3) : 2;
    sizeSeal[0] = sizeInBytes / 10u + '0';
    sizeSeal[1] = sizeInBytes % 10u + '0';
    char format[80] = "";
    strcpy(format, s);
    strcat(format, " HEX: %0");
    strcat(format, sizeSeal);
    strcat(format, PRIX64 "\n");
    printf(format, *hex);
}

typedef struct element_s {
    int bit:1;
    int mutable:1;
} element_t;
typedef element_t* element_a;

BOOL superValidHex = TRUE;
size_t superHexWidth = 0;
BOOL superFirst = TRUE;
int64_t superHex = 0x0u;

BOOL increment(element_a n, size_t size)
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
    int64_t hex = 0x0u;
    BOOL validHex = TRUE;
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
                fprintf(stderr, "unknown character %c (0x%X) in pattern %s\n", p[i], p[i], p);
                exit(2);
        }
    }

    if(superFirst) {
        superFirst = FALSE;
        superHexWidth = size;
    } else if(superHexWidth != size) {
        superValidHex = FALSE;
    }

    printf("combinations for %s:\n", p);

    while(TRUE) {
        unsigned long dec = 0;
        BOOL validDec = FALSE;
        print(n, size, &dec, &validDec);
        if(validHex && validDec && dec < sizeof(int64_t) * 8) {
            hex |= ((int64_t)1 << dec);
        } else {
            validHex = FALSE;
        }
        printf("\n");
        if(!increment(n, size)) break;
    }
    if(validHex) {
        printHex(&hex, size, "composed");
        if(superValidHex) {
            superHex |= hex;
        }
    } else {
        superValidHex = FALSE;
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
    } else if(argc == 2) {
        superValidHex = FALSE;
    }

    for(p = &argv[1]; p != last; ++p) {
        generate(*p);
    }

    if(superValidHex) {
        printHex(&superHex, superHexWidth, "all or'd");
    }

    return 0;
}
