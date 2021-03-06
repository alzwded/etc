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
   The reason for this is because I'll implement this in JakVMhs assembly
       so the code's gotta be short and effective

   compile with --v to see the magic
       gcc --std=c99 testalloc2.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// represent the SWAP operation as a macro since the JakVMhs swap
//     code bears no resemblance to C code
#define SWAPT type_t
#define SWAP(a, b) do{ SWAPT t = a; a = b; b = t; }while(0)

// represent conditional jumps with macros since assembly tends
//     to be somewhat different from structured code
#define WHEN(X) if(X)
#define JUMP(X) goto X

// configuration constants; theoretically determined at compile time
// BASEADDR: the base address of the malloc area as an offset of words; 
//           everything that's before it is static program data
#define BASEADDR (3)
// TOTAL: the total amount of memory, 64k words
//        well, actually it can be considered to be the 'right' marker
//        of the malloc area if you consider BASEADDR to be the 'left'
#define TOTAL (64*1024)
// ALLOCTAB: the size of the allocation table, in words
//           memory will be allocated starting from BASEADDR + ALLOCTAB
#define ALLOCTAB (3200)
// BLOCK: the size of an allocation block, in number of words
#define BLOCK (8)

// a representation of the memory
typedef int16_t type_t;
typedef type_t* ptr_t;
type_t memory[TOTAL];

// this would become wrapped in a
// wdalloc(int numBytes) {
//     return dalloc(numBytes / BLOCK + (numBytes % BLOCK != 0));
// }
ptr_t dalloc(int size)
{
// the variables we're using
    int addr = 0;
    size_t i = BASEADDR-1, j;

// jump into the main body of the function
    JUMP(dalloc_mainLoop);

// entered when a slot is occupied
dalloc_occupied:
    // increment our return pointer by the amount that's in the
    //     occupied slot
    addr += -memory[i];
    // fall through back into the main loop

dalloc_mainLoop:
    // look at the next slot
    ++i;
    // if it's occupied, handle it
    WHEN(memory[i] < 0)
    JUMP(dalloc_occupied);
    // if it's an exact match, handle it
    WHEN(memory[i] == size)
    JUMP(dalloc_foundMatching);
    // if there's room, handle it
    WHEN(memory[i] > size)
    JUMP(dalloc_foundMore);
    // otherwise move the potential pointer by that amount
    addr += memory[i];
    // and loop
    JUMP(dalloc_mainLoop);

// entered when an exact size match is found
dalloc_foundMatching:
    // mark it as occupied
    memory[i] = -memory[i];
    // compute the pointer address and return
    return &memory[BASEADDR + addr * BLOCK + ALLOCTAB];

// entered when a slot was found that is big enough
dalloc_foundMore:
    // save the slot's index
    j = i;
    // push back algorithm:
    //     hold the next value in a buffer
    //     while there is an array, swap the slot
    //         on the next position with the one in the buffer
    //         and increment the index
    // but first, the i+1 position will be (i)-size, so initialize
    //     the buffer with that
    type_t buf = memory[i] - size;
    // mark the current slot as occupied
    memory[i] = -size;
    // unleash the push back algorithm;
    // 0 is the terminator
dalloc_pushBackStart:
    WHEN(buf == 0)
    JUMP(dalloc_pushBackEnd);
    // move to the next slot
    ++j;
    // swap out buf and the slot
    SWAP(buf, memory[j]);
    // loop
    JUMP(dalloc_pushBackStart);
dalloc_pushBackEnd:
    // compute the pointer address and return
    return &memory[BASEADDR + addr * BLOCK + ALLOCTAB];
}

void dfree(ptr_t p)
{
// the variables we're using
    int base = (p - memory - BASEADDR - ALLOCTAB) / BLOCK;
    size_t i = BASEADDR-1, j, o = 0;

    JUMP(dfree_mainLoop);

// entered when a slot is occupied
dfree_occupied:
    // decrement our debt
    base -= memory[i];
    // fall through to dfree_mainLoop
dfree_mainLoop:
    // move to the next slot
    ++i;
    // if it's occupied, deal with it
    WHEN(memory[i] > 0)
    JUMP(dfree_occupied);
    // decrement our debt
    base -= -memory[i];
    // if we still have debt, move on
    // 0 is still debt because reasons; magic number ho
    WHEN(base >= 0)
    JUMP(dfree_mainLoop);
    // else, mark the memory as free
    memory[i] = -memory[i];
//
// normalize
//
    // if we have a free slot on the left, merge with it
    WHEN(i > BASEADDR && memory[i - 1] > 0)
    JUMP(dfree_haveLeft);
    // if we have a free slot on the right, merge with it
    WHEN(memory[i + 1] >= 0)
    JUMP(dfree_haveRight);
    // nothing to normalize
    return;

dfree_haveRight:
    // add the value of the right slot to this slot
    memory[i] += memory[i + 1];
    // jump to the move code
    j = i + 2;
    JUMP(dfree_toEndWithIt);

dfree_haveLeft:
    // add the value of this slot to the left slot
    memory[i - 1] += memory[i];
    // if we don't have a free slot to our right, skip some code
    WHEN(memory[i + 1] <= 0)
    JUMP(dfree_noFreeRight);
    // we have a free slot on the right; merge it with the left slot
    memory[i - 1] += memory[i + 1];
    // configure toEndWithIt to reach a slot farther to its right
    o = 1;
    // fall through to dfree_noFreeRight
dfree_noFreeRight:
    // configure the move code
    j = i + 1;
    // fall through to dfree_toEndWithIt
dfree_toEndWithIt:
    // if we're at the end of the malloc memory, everything was moved
    WHEN(memory[j - 1] == 0)
    JUMP(dfree_toEndWithItEnd);
    // move the slot further down the road into our house
    memory[j - 1] = memory[j + o];
    // move to the next slot
    ++j;
    // and repeat
    JUMP(dfree_toEndWithIt);
dfree_toEndWithItEnd:
    // done!
    return;
}

int main(int argc, char* argv[])
{
    memset(&memory[0], 0, sizeof(memory));
    memory[BASEADDR] = (TOTAL - ALLOCTAB - BASEADDR) / BLOCK;

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

    for(size_t i = 0; i < BASEADDR; memory[i++] = (i%2)?-9999:9999)
        ;
    SAY("+-9999 is a momentarily reserved value for testing purposes\nconsider them cannaries");

    printf("=============\n");
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

    printf("=============\n");
    a = dalloc(1);
    SAY2("a = 1", a);
    b = dalloc(1);
    SAY2("b = 1", b);
    c = dalloc(1);
    SAY2("c = 1", c);
    dfree(a);
    SAY("free a");
    dfree(c);
    SAY("free c");
    dfree(b);
    SAY("free b");

    printf("=============\n");
    a = dalloc(1);
    SAY2("a = 1", a);
    b = dalloc(1);
    SAY2("b = 1", b);
    c = dalloc(1);
    SAY2("c = 1", c);
    d = dalloc(7000);
    SAY2("d = 7000", c);
    dfree(a);
    SAY("free a");
    dfree(c);
    SAY("free c");
    dfree(b);
    SAY("free b");
    dfree(d);
    SAY("free d");

    return 0;
}
