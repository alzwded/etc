/* author: Vlad Mesco
   date: Mon May 27 22:43:20 EEST 2013
   desc: generate rom content based on ASM table

   format of input:
   line ::= SELECTOR<BITS> WHITESPACE OUTPUT<BITS> ;
   BITS ::= BITS BIT | BIT ;
   BIT ::= '0' | '1' | 'x' ;
   WHITESPACE ::= (as per isspace() cstdlib function) ;

   format of output:
   line ::= SLECTOR<XBITS> SPACE OUTPUT<XBITS> SPACE OUTPUT<HEX> ;
   XBITS ::= XBITS XBIT | XBIT ;
   XBIT ::= '0' | '1' ;
   SPACE ::= ' ' ;
   HEX ::= [0-9A-Z]+ ; // big endian
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct node_s {
    unsigned int number:1;
    unsigned int mutable:1;
    struct node_s* next;
} node_t;

typedef node_t* node_p;

typedef struct mat_line_s {
    node_p addrroot;
    node_p word;
    struct mat_line_s* next;
} mat_line_t;

typedef mat_line_t* mat_t;

void del_node(node_p p)
{
    if(p->next) del_node(p->next);
    free(p);
}

void del_mat(mat_t p)
{
    if(p->next) del_mat(p->next);
    if(p->addrroot) del_node(p->addrroot);
    if(p->word) del_node(p->word);
    free(p);
}

node_p new_node()
{
    node_p ret = (node_p)malloc(sizeof(node_t));
    ret->next = NULL;
    return ret;
}

mat_t new_mat()
{
    mat_t ret = (mat_t)malloc(sizeof(mat_line_t));
    ret->next = NULL;
    ret->addrroot = NULL;
    ret->word = NULL;
    return ret;
}

int readNum(node_p p)
{
    char c;
    while(!isspace(c = getchar()) && !feof(stdin)) {
        switch(c) {
            case '0':
                p->next = new_node();
                p = p->next;
                p->number = 0;
                p->mutable = 0;
                break;
            case '1':
                p->next = new_node();
                p = p->next;
                p->number = 1;
                p->mutable = 0;
                break;
            case 'x':
                p->next = new_node();
                p = p->next;
                p->number = 0;
                p->mutable = 1;
                break;
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
        READNUM(root);
        READNUM(wroot);

        if(!feof(stdin)) {
            pMat->next = new_mat();
            pMat = pMat->next;
            pMat->addrroot = root;
            pMat->word = wroot;
        } else {
            del_node(root);
            del_node(wroot);
        }
    }
    return mat;
}

int increment(node_p p)
{
    while(p) {
        if(!p->mutable) {
            p = p->next;
            continue;
        }
        if(!p->number) {
            p->number = 1;
            return 1;
        } else {
            p->number = 0;
            p = p->next;
            continue;
        }
    }
    return 0;
}

void printHex(node_p* p)
{
    unsigned int hex = 0x0;
    int bits = 0;
    while(bits++ < 4) {
        hex <<= 1;
        if(*p) {
            hex |= (*p)->number;
            *p = (*p)->next;
        }
    }
    printf("%1X", hex);
}

void printNum(node_p pAddr)
{
    for(pAddr = pAddr->next; pAddr; pAddr = pAddr->next) {
        printf("%1d", pAddr->number);
    }
}

void print(mat_t p)
{
    node_p pWord = p->word->next;
    printNum(p->addrroot);
    printf(" ");
    fflush(stdout);
    printNum(p->word);
    printf(" ");
    fflush(stdout);
    for(; pWord; printHex(&pWord));
    printf("\n");
    fflush(stdout);
}

void f()
{
    mat_t mat = readStuff();
    mat_t p = mat->next;
    
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
