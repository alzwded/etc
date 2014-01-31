#include <stdio.h>
// base 16 string to int
int matoi16(char const* volatile s)
{
    // make local variables volatile because the output with -O2 or
    //    -O3 was causing strings to become NULL for whatever reason
    // The s pointer is copied for the very same reason
    char const* volatile s1 = s;
    int volatile ret;
    asm volatile (   
            "xorl %%eax, %%eax\n\t"    // upper three bytes -> 0
            "xorl %0, %0\n\t"       // ret = 0

            "matoi16_loop%=:\n\t"     // while(*s1) {
            "movb (%1), %%al\n\t"
            "cmpl $0, %%eax\n\t"
            "jz matoi16_end%=\n\t"
            "shll $4, %0\n\t"       //      ret <<= 4

            "cmpl $0x61, %%eax\n\t" //      if(*s1 > 'a') goto small_letters
            "jge matoi16_small_letters%=\n\t"

            "cmpl $0x41, %%eax\n\t" //      if(*s1 > 'A') goto big_letters
            "jge matoi16_big_letters%=\n\t"

            "subl $0x30, %%eax\n\t" //      eax = *s1 - '0'
            "matoi16_sum%=:\n\t"
            "addl %%eax, %0\n\t"    //      ret += eax
            "inc %1\n\t"            //      s1++
            "jmp matoi16_loop%=\n\t"

            "matoi16_big_letters%=:\n\t"
            "subl $0x37, %%eax\n\t" //      eax = *s1 - 'A'
            "jmp matoi16_sum%=\n\t"

            "matoi16_small_letters%=:\n\t"
            "subl $0x57, %%eax\n\t" //      eax = *s1 - 'a'
            "jmp matoi16_sum%=\n\t"
            "matoi16_end%=:\n\t"
            : "=&r"(ret)
            : "r"(s1)
            : "%eax"
        );
    return ret;
}

// base 8 string to int
int matoi8(char const* volatile s)
{
    // make local variables volatile because the output with -O2 or
    //    -O3 was causing strings to become NULL for whatever reason
    // The s pointer is copied for the very same reason
    char const* volatile s1 = s;
    int volatile ret;
    asm volatile (   
            "xorl %%eax, %%eax\n\t"    // upper three bytes -> 0
            "xorl %0, %0\n\t"       // ret = 0

            "matoi8_loop%=:\n\t"      // while(*s1) {
            "movb (%1), %%al\n\t"
            "cmpl $0, %%eax\n\t"
            "jz matoi8_end%=\n\t"
            "shll $3, %0\n\t"       //      ret <<= 3

            "subl $0x30, %%eax\n\t" //      eax = *s1 - '0'
            "matoi8_sum%=:\n\t"
            "addl %%eax, %0\n\t"    //      ret += eax
            "inc %1\n\t"            //      s1++
            "jmp matoi8_loop%=\n\t"

            "matoi8_end%=:\n\t"
            : "=&r"(ret)             // output ret, W/O random register
            : "r"(s1)               // input s1, R/W random register
            : "%eax"                 // modifying eAX
        );
    return ret;
}

int main()
{
    char const*  s1 = "0127F";
    char const*  s2 = "FF";
    char const*  s3 = "C0";
    char const*  s4 = "0C";

    char const*  s5 = "0123";
    char const*  s6 = "10";
    char const*  s7 = "777";

    printf("str -> matoi16(str) // should be equal in value\n");
    printf("%5s = %4X = %d (10)\n", s1, matoi16(s1), matoi16(s1));
    printf("%5s = %4X = %d (10)\n", s2, matoi16(s2), matoi16(s2));
    printf("%5s = %4X = %d (10)\n", s3, matoi16(s3), matoi16(s3));
    printf("%5s = %4X = %d (10)\n", s4, matoi16(s4), matoi16(s4));

    printf("str -> matoi8(str) // should be equal in value\n");
    printf("%5s = %4o = %d (10)\n", s5, matoi8(s5), matoi16(s5));
    printf("%5s = %4o = %d (10)\n", s6, matoi8(s6), matoi16(s6));
    printf("%5s = %4o = %d (10)\n", s7, matoi8(s7), matoi16(s7));

    return 0;
}
