          #include               <stdio.h>
        #         include           <stdlib.h>
      #             include             <ctype.h>
    #define         READNUM        (   P   )  do{\
    if   (!         readNum      (  (  P  )  ) ) {\
        del_node(root)                            ;\
       del_node(wroot)                             ;\
      del_mat(                                    mat\
     );exit(10)                                      ;\
    }}while(0                                          )
    typedef struct node_s{char number:1; char mutable:1;
    struct node_s* next;}node_t; typedef node_t* node_p;
    typedef struct mat_line_s { node_p addrroot ; node_p
    word; struct mat_line_s* next; } mat_line_t; typedef
    mat_line_t* mat_t;__inline__ void del_node(node_p p)
    { if(p->next) del_node(p->next);free(p);} __inline__
    void del_mat(mat_t p) { if(p->next) del_mat(p->next)
    ;if(p->addrroot) del_node(p->addrroot); if (p->word)
    del_node(p->word); free(p); } inline node_p new_node
    (){ node_p ret = (node_p)malloc(sizeof(node_t)); ret
    ->next = NULL; return ret;} __inline__ mat_t new_mat
    () { mat_t ret = (mat_t) malloc(sizeof(mat_line_t));
    ret->next = (mat_t)(ret->addrroot=ret->word = NULL);
    return ret; } char readNum(node_p p) { char c; while
    (!isspace(c = getchar()) && !feof(stdin)) {switch(c)
    {case'0':case '1':case 'x':p = p->next = new_node();
    p->number= (c == '1'); p->mutable= (c == 'x');break;
    default:fprintf(stderr, "unknown character %c\n",c);
    return 0; } } return 1 ; } mat_t readStuff() { mat_t
    mat = new_mat();mat_t pMat = mat;while(!feof(stdin))
    {node_p root = new_node();node_p wroot = new_node();
    READNUM(root); READNUM(wroot); if(!feof(stdin)){pMat
    =(pMat->next = new_mat());pMat->addrroot = root;pMat
    ->word = wroot;}else{del_node(root);del_node(wroot);
    }} return mat; } __inline__ char increment(node_p p)
    {return (p != NULL)&&(!((!p->mutable) || !(p->number
    =!p->number))? 1:increment(p->next));} void printHex
    (node_p* p) { char hex = 0x0; char bits = 0x0; while
    ( bits++ < 4) { hex <<= 1; if(!*p) continue; hex |=(
    *p)->number; *p = (*p)->next; } printf("%1X", hex);}
    __inline__ void printNum(node_p pAddr) { for(; pAddr
    ->next printf("%1d",pAddr->next->number),pAddr=pAddr
    ->next); } void print (mat_t p) { node_p pWord = p->
    word->next; printNum ( p->addrroot ) ; printf (" ");
    printNum(p->word); printf(" "); for(;pWord;printHex(
    &pWord)); printf("\n");} __inline__ void f() { mat_t
    mat = readStuff(); mat_t p = mat->next; for(;p;print
    (p), increment(p->addrroot) || (p= p->next));del_mat
    (mat); } int main(int argc, char* argv []) { if(argc
    > 2) { fprintf(stderr, "usage: %s [infile]\n", argv[
    0]); return 1; } if(argc                   == 2) {
    if    (!freopen(argv[1], "r", stdin           )) {
            fprintf(stderr, "cannot open %s\n"       ,
                  argv[1]); exit(2); } } f()        ;
                  return         0       ; }
