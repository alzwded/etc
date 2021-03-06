#include <cstdio>
#include "jakcode.h"
#include <memory>

int main()
{
    FILE* f = fopen("test.in", "rb");
    FILE* g = fopen("test.out", "wb");

    jakcode_stream_encode(f, g);
    
    f = fopen("test.out", "rb");
    g = fopen("test.reout", "wb");

    jakcode_stream_decode(f, g);

    const unsigned char* text = (const unsigned char*)"Hello\nHello";
    int stext = 12;
    std::unique_ptr<unsigned char> poutput;
    std::unique_ptr<unsigned char> predone;
    unsigned char* output;
    unsigned char* redone;
    int sdone, soutput;

    printf("input:\n%s\n(end of input)\n", text);
    jakcode_mem_encode(text, stext, &output, &soutput);
    poutput.reset(output);
    jakcode_mem_decode(output, soutput, &redone, &sdone);
    predone.reset(redone);
    printf("output:\n%s\n(end of output)\n", redone);

    return 0;
}
