#include "encoder.h"
#include "decoder.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "main.i"

void encode_fin(const char* s)
{
    FILE* f = fopen(s, "rb");
    FILE* g = fdopen(1, "ab");
    jakcode_stream_encode(f, g);
}

void decode_fin(const char* s)
{
    FILE* f = fopen(s, "rb");
    FILE* g = fdopen(1, "ab");
    jakcode_stream_decode(f, g);
}

void encode_fin_fout(char* s)
{
    char* other = (char*)malloc(strlen(s));
    int i;
    FILE* f;
    FILE* g;
    for(i = 0; i < strlen(s); ++i)
    {
        if(s[i] == ',') { other[i] = (char)0; s = &s[i + 1]; break; }
        other[i] = s[i];
    }
    f = fopen(other, "rb");
    g = fopen(s, "wb");
    jakcode_stream_encode(f, g);
}

void decode_fin_fout(char* s)
{
    char* other = (char*)malloc(strlen(s));
    int i;
    FILE* f;
    FILE* g;
    for(i = 0; i < strlen(s); ++i)
    {
        if(s[i] == ',') { other[i] = (char)0; s = &s[i + 1]; break; }
        other[i] = s[i];
    }
    f = fopen(other, "rb");
    g = fopen(s, "wb");
    jakcode_stream_decode(f, g);
}

void encode_fout(const char* s)
{
    FILE* f = fopen(s, "wb");
    FILE* g = fdopen(0, "rb");
    jakcode_stream_encode(g, f);
}

void decode_fout(const char* s)
{
    FILE* f = fopen(s, "wb");
    FILE* g = fdopen(0, "rb");
    jakcode_stream_decode(g, f);
}

int main(int argc, char* argv[]) {
    if(argc != 2) { abort(); }
    if(strlen(argv[1]) < 2) { abort(); }
    if(argv[1][1] == 'e') {
        jakcode_stream_encode(fdopen(0, "rb"), fdopen(1, "ab"));
    } else if(argv[1][1] == 'd') {
        jakcode_stream_decode(fdopen(0, "rb"), fdopen(1, "ab"));
    } else if(argv[1][1] == 'E') {
        encode_fin(&argv[1][2]);
    } else if(argv[1][1] == 'D') {
        decode_fin(&argv[1][2]);
    } else if(argv[1][1] == 'f') {
        encode_fin_fout(&argv[1][2]);
    } else if(argv[1][1] == 'F') {
        decode_fin_fout(&argv[1][2]);
    } else if(argv[1][1] == 'g') {
        encode_fout(&argv[1][2]);
    } else if(argv[1][1] == 'G') {
        decode_fout(&argv[1][2]);
    } else if(argv[1][1] == 'h') {
        printf("usage: %s -edEDfFGgh\ne - encode stdin to stdout\nd - decode stdin to stdout\nEfile - encode file to stdout\nDpath - decode file to stdout\nfpath,path - encode file to file\nFpath,path - decode file to file\ngpath - encode stdin to file\nGpath - decode stdin to path\n", argv[0]);
        return 1;
    } else {
        abort();
    }
    return 0;
}
