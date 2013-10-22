#ifndef COMMON_H
#define COMMON_H
#include <stddef.h>
typedef struct {
    unsigned char* s;
    size_t n;
} str_t;
unsigned int uv(unsigned char c);
#endif
