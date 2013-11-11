#include "common.h"
unsigned int uv(unsigned char c)
{
    unsigned int ret = 0;
    ret |= (unsigned)c & 0xFF;
    return ret;
}
