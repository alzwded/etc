#include "decoder.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>

str_t jakcode_despace(str_t s)
{
    str_t ret = {
        (unsigned char*)malloc(sizeof(unsigned char) * s.n + 1),
        0 };
    size_t i, j;
    unsigned int otherCount = 0;
    unsigned int count = 0;
    for(i = 0, j = 0; i < s.n; ++i, otherCount++)
    {
        count += uv(s.s[i]);
        ret.s[j++] = s.s[i];
        if(count >= 800) {
            count = 0;
            otherCount++;
            ++i;
        }
        if(otherCount >= 80) {
            ++i;
            otherCount = 0;
        }
    }
    ret.s[j] = (unsigned char)0;
    ret.n = j;
    return ret;
}

static void jakcode_flip(str_t s)
{
    size_t i;
    size_t n = s.n - 1;
    for(i = 0; i < n / 2 + (n % 2); ++i) 
    {
        s.s[i] = s.s[i] ^ s.s[n - i];
        s.s[n - i] = s.s[i] ^ s.s[n - i];
        s.s[i] = s.s[i] ^ s.s[n - i];
    }
}

str_t jakcode_decode(str_t s)
{
    str_t ret = {
        (unsigned char*)malloc(sizeof(unsigned char) * s.n + 1),
        0 };
    size_t i, j = 0;
    for(i = s.n - 1, j = 0; i > 0; --i)
    {
        if(s.s[i] > 127) {
            ret.s[j++] = (unsigned char)
                (128 + ((uv(s.s[i]) + uv(s.s[i - 1])) % 128));
        } else {
            ret.s[j++] = (unsigned char)
                ((128 + uv(s.s[i]) - uv(s.s[i - 1])) % 128);
        }
    }
    ret.s[j++] = (256 + s.s[0] - 2) % 256;
    ret.s[j] = (unsigned char)0;
    ret.n = j;

    free(s.s);
    jakcode_flip(ret);
    return ret;
}

str_t jakcode_depad(str_t s)
{
    str_t ret = {
        (unsigned char*)malloc(sizeof(unsigned char) * s.n + 1),
        0 };
    size_t i, j;
    j = 0;
    for(i = 0; i < s.n; ++i)
    {
        if(s.s[i] == ((unsigned char)~((unsigned char)'_'))) {
            ++i;
            if(s.s[i] == ((unsigned char)~((unsigned char)'_'))) {
                ret.s[j++] = ((unsigned char)' ');
            } else if(s.s[i] == ((unsigned char)~((unsigned char)'n'))) {
                ret.s[j++] = (unsigned char)10;
            } else if(s.s[i] == ((unsigned char)~((unsigned char)'u'))) {
                ret.s[j++] = ((unsigned char)'_');
            } else {
                ret.s[j++] = ~s.s[i];
                abort();
            }
            continue;
        }
        ret.s[j++] = ~s.s[i];
    }
    ret.s[j] = (unsigned char)0;
    ret.n = j;
    free(s.s);
    return ret;
}
