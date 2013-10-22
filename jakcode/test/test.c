#include <stdio.h>
#include "jakcode.h"

const unsigned char* text = "Hello\nHello";
int stext = 12;
unsigned char* output;
int soutput;
unsigned char* redone;
int sdone;

int main()
{
    FILE* f = fopen("test.in", "rb");
    FILE* g = fopen("test.out", "wb");

    jakcode_stream_encode(f, g);
    
    f = fopen("test.out", "rb");
    g = fopen("test.reout", "wb");

    jakcode_stream_decode(f, g);

    printf("input:\n%s\n(end of input)\n", text);
    jakcode_mem_encode(text, stext, &output, &soutput);
    jakcode_mem_decode(output, soutput, &redone, &sdone);
    printf("output:\n%s\n(end of output)\n", redone);

    return 0;
}
