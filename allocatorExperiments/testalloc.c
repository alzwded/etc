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
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef int8_t type_t;
typedef type_t* ptr_t;

type_t memory[4096];

ptr_t dalloc(int size)
{
    int addr = 0;
    size_t i = 0;
    for(i = 0; i < 4096; ++i) {
        if(memory[i] == 0) {
            memory[i] = -size;
            return &memory[addr * 8 + 200];
        } else if(memory[i] < 0) {
            addr += -memory[i];
            continue;
        } else if(memory[i] > 0) {
            if(memory[i] == size) {
                memory[i] = -memory[i];
                return &memory[addr * 8 + 200];
            } else if(memory[i] > size) {
                // move everything back
                int j = i + 1;
                while(memory[j] != 0) ++j;
                for(; j > i; --j) {
                    memory[j] = memory[j - 1];
                }
                memory[i] = -size;
                memory[i + 1] =  memory[i + 1] - size;
                return &memory[addr * 8 + 200];
            } else {
                addr += memory[i];
                continue;
            }
        }
    }
    if(i >= 4096) abort();
}

void dfree(ptr_t p)
{
    int base = (p - memory - 200) / 8;
    for(size_t i = 0; i < 4096; ++i) {
        if(memory[i] == 0) abort();
        if(memory[i] < 0) {
            base -= -memory[i];
            if(base < 0) {
                memory[i] = -memory[i];
                // normalize
                if(i > 0 && memory[i - 1] > 0
                    && memory[i + 1] > 0)
                {
                    memory[i - 1] += memory[i] + memory[i + 1];
                    size_t j = i + 1;
                    for(; memory[j] && memory[j + 1]; ++j) {
                        memory[j - 1] = memory[j + 1];
                    }
                    memory[j - 1] = 0;
                    memory[j] = 0;
                } else if(i > 0 && memory[i - 1] > 0
                    && memory[i + 1] <= 0)
                {
                    memory[i - 1] += memory[i];
                    size_t j = i + 1;
                    for(; memory[j]; ++j) {
                        memory[j - 1] = memory[j];
                    }
                    memory[j - 1] = 0;
                } else if(memory[i + 1] > 0) {
                    memory[i] += memory[i + 1];
                    size_t j = i + 1;
                    for(; memory[j + 1]; ++j) {
                        memory[j] = memory[j + 1];
                    }
                    memory[j] = 0;
                } else if(memory[i + 1] == 0) {
                    memory[i] = 0;
                } else {
                    break; // nothing more to normalize
                }
                break;
            }
        } else if(memory[i] > 0) {
            base -= memory[i];
        }
    }

    if(memory[0] > 0 && memory[1] == 0) {
        memory[0] = 0;
    }
}

int main(int argc, char* argv[])
{
    memset(&memory[0], 0, sizeof(memory));

#define SAY(STR) do{\
    printf("%s\n", STR); \
    for(size_t i = 0; i < 10; ++i) printf("%3d", memory[i]); \
    printf("\n"); \
}while(0)

#define SAY2(STR, VAR) do{\
    printf("%s\n", STR); \
    for(size_t i = 0; i < 10; ++i) printf("%3d", memory[i]); \
    printf("\n"); \
    printf("         %d\n", VAR - memory); \
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
