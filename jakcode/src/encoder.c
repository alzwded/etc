#include "encoder.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>

static unsigned jakcode_countLF(const str_t s)
{
    size_t i = 0;
    unsigned ret = 0;
    for(; i < s.n; ++i)
    {
        if(s.s[i] == (unsigned char)10) { ret++; continue; }
        if(s.s[i] == ' ') { ret++; continue; }
        if(s.s[i] == '_') { ret++; continue; }
    }
    return ret;
}

str_t jakcode_pad(str_t s)
{
    int nbLineFeeds = jakcode_countLF(s);
    str_t ret = {
            (unsigned char*)
                malloc((s.n + nbLineFeeds) * sizeof(unsigned char) + 1),
            0 };
    size_t i, j;
    for(i = 0, j = 0; i < s.n; ++i) {
        if(s.s[i] == ' ') {
            ret.s[j++] = ((unsigned char)~((unsigned char)'_'));
            ret.s[j++] = ((unsigned char)~((unsigned char)'_'));
            continue;
        } else if(s.s[i] == (unsigned char)10) {
            ret.s[j++] = ((unsigned char)~((unsigned char)'_'));
            ret.s[j++] = ((unsigned char)~((unsigned char)'n'));
            continue;
        } else if(s.s[i] == '_') {
            ret.s[j++] = ((unsigned char)~((unsigned char)'_'));
            ret.s[j++] = ((unsigned char)~((unsigned char)'u'));
            continue;
        } else {
            ret.s[j++] = ~s.s[i];
        }
    }
    ret.s[j] = (unsigned char)0;
    ret.n = j;
    free(s.s);
    return ret;
}

str_t jakcode_encode(str_t s)
{
    size_t i;
    str_t ret = {
        (unsigned char*)malloc(sizeof(unsigned char) * s.n + 1),
        0 };
    ret.s[0] = (uv(s.s[0]) + 2) % 256;
    for(i = 1; i < s.n; ++i)
    {
        if(s.s[i] > 127) {
            ret.s[i] = (unsigned char)
                128 + ((256 - uv(ret.s[i - 1]) + uv(s.s[i])) % 128);
        } else {
            ret.s[i] = (unsigned char)
                ((uv(ret.s[i - 1]) + uv(s.s[i])) % 128);
        }
    }
    ret.s[i] = (unsigned char)0;
    ret.n = i;
    free(s.s);
    return ret;
}

str_t jakcode_space(str_t s)
{
    str_t ret = {
        (unsigned char*)malloc(sizeof(unsigned char) * 3 * s.n + 1),
        0 };
    size_t i, j;
    unsigned int count = 0;
    unsigned int otherCount = 0;

    for(i = 0, j = 0; i < s.n; ++i, otherCount++) {
        count += uv(s.s[i]);
        ret.s[j++] = s.s[i];
        if(count >= 800) {
            ret.s[j++] = ' ';
            otherCount++;
            count = 0;
        }
        if(otherCount >= 80) {
            ret.s[j++] = (unsigned char)10;
            otherCount = 0;
        }
    }
    ret.s[j] = (unsigned char)0;
    ret.n = j;

    free(s.s);
    return ret;
}
