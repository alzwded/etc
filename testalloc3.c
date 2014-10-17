/* Author: Vlad Mesco
   Date:   15/10/14
   Description:
       Simulate a very simple implementation of malloc that
       operates on 4096 bytes, reserving the first 200 for the
       allocation table.
       The allocation table contains full, empty and untouched areas.
       The alloc implementation takes a "first fit" approach.
       The free implementation attempts to normalize the empty areas
       to help keep the alloc'er simple.
       I should be able to use this as a base for an malloc
       implementation over on my JakVMhs project

   Update from testalloc.c:
       make it use short's since that's what I'll be using for JakVMhs
       with this in mind, I can now modify it a bit to store the number
       of blocks free at the end of the list instead of magical 0s

   Simplified less error-checky version of testalloc2.c

   compile with --v to see the magic
       gcc --std=c99 testalloc2.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef int16_t type_t;
typedef type_t* ptr_t;

#define TOTAL (64*1024)//(4096)
#define ALLOCTAB (3200)//(200)
#define BLOCK (8)

#define SWAPT type_t
#define SWAP(a, b) do{ SWAPT t = a; a = b; b = t; }while(0)

type_t memory[TOTAL];

// this would become wrapped in a
// wdalloc(int numBytes) {
//     return dalloc(numBytes / BLOCK + (numBytes % BLOCK != 0));
// }
ptr_t dalloc(int size)
{
    int addr = 0;
    size_t i = 0;
    for(i = 0; 1; ++i) {
        if(memory[i] < 0) {
            addr += -memory[i];
            continue;
        } else if(memory[i] == size) {
            memory[i] = -memory[i];
            return &memory[addr * BLOCK + ALLOCTAB];
        } else if(memory[i] > size) {
            // move everything back
            int j = i;
            type_t buf = memory[i] - size;
            memory[i] = -size;
            while(buf != 0) {
                ++j;
                SWAP(buf, memory[j]);
            }
            return &memory[addr * BLOCK + ALLOCTAB];
        } else {
            addr += memory[i];
            continue;
        }
    }
    // no code here
}

void dfree(ptr_t p)
{
    int base = (p - memory - ALLOCTAB) / BLOCK;
    size_t j, o = 0;
    for(size_t i = 0; 1; ++i) {
        if(memory[i] > 0) {
            base -= memory[i];
            continue;
        }
        base -= -memory[i];
        if(base >= 0) continue;

        memory[i] = -memory[i];

        // normalize
        if(i > 0 && memory[i - 1] > 0) {
            memory[i - 1] += memory[i];
            if(memory[i + 1] > 0) {
                memory[i - 1] += memory[i + 1];
                j = i + 1;
                o = 1;
                goto toEndWithIt;
            } else {
                j = i + 1;
                goto toEndWithIt;
            }
        } else if(memory[i + 1] >= 0) {
            memory[i] += memory[i + 1];
            j = i + 2;
            goto toEndWithIt;
        }
        return;
    }
    // no code here

toEndWithIt:
    for(; memory[j - 1]; ++j) {
        memory[j - 1] = memory[j + o];
    }
    return;
}

int main(int argc, char* argv[])
{
    memset(&memory[0], 0, sizeof(memory));
    memory[0] = (TOTAL - ALLOCTAB) / BLOCK;

#define SAY(STR) do{\
    printf("%s\n", STR); \
    for(size_t i = 0; i < 10; ++i) printf("%5d", (int)memory[i]); \
    printf("\n"); \
}while(0)

#define SAY2(STR, VAR) do{\
    printf("%s\n", STR); \
    for(size_t i = 0; i < 10; ++i) printf("%5d", (int)memory[i]); \
    printf("\n"); \
    printf("    PWORD%5d PCHAR%5d\n", VAR - memory, (char*)VAR - (char*)memory); \
}while(0)

    SAY("initial");
    ptr_t a = dalloc(1);
    SAY2("a = 1 block", a);
    ptr_t b = dalloc(3);
    SAY2("b = 3", b);
    ptr_t c = dalloc(3);
    SAY2("c = 3", c);
    ptr_t d = dalloc(1);
    SAY2("d = 1", d);
    ptr_t e = dalloc(4);
    SAY2("e = 4", e);
    dfree(c);
    SAY("free'd c (3)");
    dfree(d);
    SAY("free'd d (1)");
    ptr_t f = dalloc(5);
    SAY2("f = 5", f);
    ptr_t g = dalloc(2);
    SAY2("g = 2", g);
    ptr_t h = dalloc(2);
    SAY2("h = 2", h);
    dfree(h);
    SAY("free'd h");
    dfree(g);
    SAY("free'd g");

    g = dalloc(2);
    SAY("allocated g again");
    h = dalloc(2);
    SAY("allocated h again");
    dfree(g);
    SAY("free'd g");
    dfree(h);
    SAY("free'd h");


    dfree(f);
    SAY("free'd f");
    dfree(b);
    SAY("free'd b");
    dfree(a);
    SAY("free'd a");
    dfree(e);
    SAY("free'd e");

    return 0;
}
