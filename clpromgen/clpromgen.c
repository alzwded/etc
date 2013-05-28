/* author: Vlad Mesco
   date: Mon May 27 22:43:20 EEST 2013
   desc: generate rom content based on ASM table

   format of input:
   line ::= SELECTOR<BITS> WHITESPACE OUTPUT<BITS> ;
   BITS ::= BITS BIT | BIT ;
   BIT ::= '0' | '1' | 'x' ;
   WHITESPACE ::= (as per isspace() cstdlib function) ;

   format of output:
   output ::= output line '\n' | line '\n' ;
   line ::= SLECTOR<XBITS> SPACE OUTPUT<XBITS> SPACE OUTPUT<HEX> ;
   XBITS ::= XBITS XBIT | XBIT ;
   XBIT ::= '0' | '1' ;
   SPACE ::= ' ' ;
   HEX ::= [0-9A-Z]+ ; // big endian
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// linked list to store a trit
//    mutable means a DC value
typedef struct node_s {
    char number:1;
    char mutable:1;
    struct node_s* next;
} node_t;

typedef node_t* node_p;

// store the input table
typedef struct mat_line_s {
    node_p addrroot;
    node_p word;
    struct mat_line_s* next;
} mat_line_t;

typedef mat_line_t* mat_t;

// memory management of the table
__inline__ void del_node(node_p p)
{
    if(p->next) del_node(p->next);
    free(p);
}

__inline__ void del_mat(mat_t p)
{
    if(p->next) del_mat(p->next);
    if(p->addrroot) del_node(p->addrroot);
    if(p->word) del_node(p->word);
    free(p);
}

__inline__ node_p new_node()
{
    node_p ret = (node_p)malloc(sizeof(node_t));
    ret->next = NULL;
    return ret;
}

__inline__ mat_t new_mat()
{
    mat_t ret = (mat_t)malloc(sizeof(mat_line_t));
    ret->next = (mat_t)(ret->addrroot = ret->word = NULL);
    return ret;
}

// read a trinary number
char readNum(node_p p)
{
    char c;
    // read a continuous sequence of trits
    while(!isspace(c = getchar()) && !feof(stdin)) {
        switch(c) {
            // spawn a new trit and move to it, assigning meaningful values
            case '0':
            case '1':
            case 'x':
                p = p->next = new_node();
                p->number = (c == '1');
                p->mutable = (c == 'x');
                break;
            // input sanity check
            default:
                fprintf(stderr, "unknown character %c\n", c);
                return 0;
        }
    }
    return 1;
}

mat_t readStuff()
{
    mat_t mat = new_mat();
    mat_t pMat = mat;
    while(!feof(stdin)) {
        // spawn two numbers
        node_p root = new_node();
        node_p wroot = new_node();
#define READNUM(P) do{\
    if(!readNum( (P) )) {\
        del_node(root);\
        del_node(wroot);\
        del_mat(mat);\
        exit(10);\
    }\
}while(0)
        // read two nums
        READNUM(root);
        READNUM(wroot);

        // if end of input was reached and it was not EOL terminated,
        //     then the input was probably created on windows and we
        //     don't like that, so we reject the last line
        // either that, or there was an error
        if(!feof(stdin)) {
            pMat = (pMat->next = new_mat());
            pMat->addrroot = root;
            pMat->word = wroot;
        } else {
            del_node(root);
            del_node(wroot);
        }
    }
    return mat;
}

// increment the mutable part of the number
// returns 0 on overflow or on bad input or if there was nothing to increment
__inline__ char increment(node_p p)
{
    return (p != NULL) && (!((!p->mutable) || !(p->number = !p->number)) ? 1 : increment(p->next));
}

// squash 4bits into a hex reprezentation and print it
void printHex(node_p* p)
{
    char hex = 0x0;
    char bits = 0;
    while(bits++ < 4) {
        hex <<= 1;
        if(!*p) continue;
        hex |= (*p)->number;
        *p = (*p)->next;
    }
    printf("%1X", hex);
}

// print a binary number
__inline__ void printNum(node_p pAddr)
{
    for(; pAddr->next; printf("%1d", pAddr->next->number), pAddr = pAddr->next);
}

// print a line of the output table
void print(mat_t p)
{
    node_p pWord = p->word->next;
    printNum(p->addrroot);
    printf(" ");
    printNum(p->word);
    printf(" ");
    for(; pWord; printHex(&pWord));
    printf("\n");
}

__inline__ void f()
{
    mat_t mat = readStuff();
    mat_t p = mat->next;
    
    // while we have not reached the end of the input table,
    //     print out a line of the output table and then
    //     either increment the address part or move to the next line
    for(; p; print(p), increment(p->addrroot) || (p = p->next));

    del_mat(mat);
}

int main(int argc, char* argv[])
{
    if(argc > 2) {
        fprintf(stderr, "usage: %s [infile]\n", argv[0]);
        return 1;
    }
    if(argc == 2) {
        if(!freopen(argv[1], "r", stdin)) {
            fprintf(stderr, "cannot open %s\n", argv[1]);
            exit(2);
        }
    }
    f();
    return 0;
}
