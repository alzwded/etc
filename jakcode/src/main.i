
void jakcode_stream_encode(FILE* fin, FILE* fout)
{
    int i;
    char* s;
    int c;
    // foreach 8096 byte block
    do {
        i = 0;
        s = (char*)malloc(8096 * sizeof(char));
        for(; i < 8096 && ((c = fgetc(fin)) != EOF); ++i) {
            s[i] = c;
        }
        str_t input = {s, i};
        str_t output = jakcode_space(jakcode_encode(jakcode_pad(input))); 
        fwrite(&output.n, sizeof(size_t), 1, fout);
        fwrite(output.s, sizeof(unsigned char), output.n, fout);
        free(output.s);
    }while(!feof(fin));
    fclose(fout);
    fclose(fin);
}
void jakcode_stream_decode(FILE* fin, FILE* fout)
{
    int i, c;
    char* s;
    // foreach 8096 byte block
    // TODO prepend with size of block
    do {
        size_t sizeOfBlock;
        fread(&sizeOfBlock, sizeof(size_t), 1, fin);
        i = 0;
        s = (char*)malloc(sizeOfBlock * sizeof(char));
        //fread(&s, sizeof(unsigned char), sizeOfBlock, fin); // does bad things on EOF
        for(; i < 8096 && ((c = fgetc(fin)) != EOF); ++i) {
            s[i] = c;
        }
        str_t input = {s, i};
        str_t output = jakcode_depad(jakcode_decode(jakcode_despace(input)));
        fwrite(output.s, sizeof(unsigned char), output.n, fout);
        free(output.s);
    }while(!feof(fin));
    fclose(fout);
    fclose(fin);
}

struct node_t {
    str_t data;
    struct node_t* next;
};

unsigned char* squash(struct node_t* out_pieces, int size)
{
    unsigned char* ret = (unsigned char*)malloc(sizeof(char) * size);
    unsigned char* p = ret;
    int soFar = 0;
    struct node_t* current = out_pieces;
    while(current) {
        memcpy(p, current->data.s, current->data.n);
        soFar += current->data.n;
        p = &p[current->data.n];
        current = current->next;
    }
    return ret;
}

void jakcode_mem_encode(const unsigned char* in, int sin, unsigned char** out, int* sout)
{
    struct node_t* out_pieces = (struct node_t*)malloc(sizeof(struct node_t));
    struct node_t* current = NULL;
    const unsigned char* p = in;
    out_pieces->data.s = NULL;
    out_pieces->data.n = 0;
    out_pieces->next = NULL;
    *sout = 0;
    while(sin > 0) {
        unsigned char* s = (unsigned char*)malloc(sizeof(char) * 3 * 8096);
        int size = ((sin < 8096) ? sin : 8096);
        if(current == NULL) { current = out_pieces; }
        else {
            current->next = (struct node_t*)malloc(sizeof(struct node_t));
            current = current->next;
            current->data.s = NULL;
            current->data.n = 0;
            current->next = NULL;
        }
        memcpy(s, p, size);
        str_t input = { s, size };
        p = &p[size];
        sin -= size;
        //*sout += size;
        current->data = jakcode_space(jakcode_encode(jakcode_pad(input)));
        *sout += current->data.n;
    }
    *out = squash(out_pieces, *sout);
}

void jakcode_mem_decode(const unsigned char* in, int sin, unsigned char** out, int* sout)
{
    struct node_t* out_pieces = (struct node_t*)malloc(sizeof(struct node_t));
    struct node_t* current = NULL;
    const unsigned char* p = in;
    out_pieces->data.s = NULL;
    out_pieces->data.n = 0;
    out_pieces->next = NULL;
    *sout = 0;
    while(sin > 0) {
        unsigned char* s = (unsigned char*)malloc(sizeof(char) * 8096);
        int size = ((sin < 8096) ? sin : 8096);
        if(current == NULL) { current = out_pieces; }
        else {
            current->next = (struct node_t*)malloc(sizeof(struct node_t));
            current = current->next;
            current->data.s = NULL;
            current->data.n = 0;
            current->next = NULL;
        }
        memcpy(s, p, size);
        str_t input = { s, size };
        p = &p[size];
        sin -= size;
        *sout += size;
        current->data = jakcode_depad(jakcode_decode(jakcode_despace(input)));
    }
    *out = squash(out_pieces, *sout);
}
