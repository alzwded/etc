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

// base 10 atoi
int matoi10(char const* volatile s)
{
    char const* s1 = s;
    int volatile ret;
    asm volatile(
            "xorl %%eax, %%eax\n\t"    // upper three bytes -> 0
            "xorl %0, %0\n\t"       // ret = 0

            "matoi10_loop%=:\n\t"      // while(*s1) {
            "movb (%1), %%al\n\t"
            "cmpl $0, %%eax\n\t"
            "jz matoi10_end%=\n\t"
            "movl %0, %%ebx\n\t"        // ret *= 10
            "shll $1, %%ebx\n\t"
            "shll $3, %0\n\t"
            "addl %%ebx, %0\n\t"

            "subl $0x30, %%eax\n\t" //      eax = *s1 - '0'
            "matoi10_sum%=:\n\t"
            "addl %%eax, %0\n\t"    //      ret += eax
            "inc %1\n\t"            //      s1++
            "jmp matoi10_loop%=\n\t"

            "matoi10_end%=:\n\t"
            : "=&r"(ret)             // output ret, W/O random register
            : "r"(s1)               // input s1, R/W random register
            : "%eax", "%ebx"         // modifying eAX, ebx
        );
    return ret;
}

// how to use MUL:
/*
   movl $10, %ebx
   movl $10, %eax
   mul %eax
   ; result is in edx:eax
   ; in this case OF and CF are 0 (no overflow), edx is 0 and eax is 100
*/

// short versions:
// base 8 string to short
short satoi8(char const* volatile s)
{
    // make local variables volatile because the output with -O2 or
    //    -O3 was causing strings to become NULL for whatever reason
    // The s pointer is copied for the very same reason
    char const* volatile s1 = s;
    short volatile ret;
    asm volatile (   
            "xorw %%ax, %%ax\n\t"    // upper three bytes -> 0
            "xorw %0, %0\n\t"       // ret = 0

            "satoi8_loop%=:\n\t"      // while(*s1) {
            "movb (%1), %%al\n\t"
            "cmpw $0, %%ax\n\t"
            "jz satoi8_end%=\n\t"
            "shlw $3, %0\n\t"       //      ret <<= 3

            "subw $0x30, %%ax\n\t" //      eax = *s1 - '0'
            "satoi8_sum%=:\n\t"
            "addw %%ax, %0\n\t"    //      ret += eax
            "inc %1\n\t"            //      s1++
            "jmp satoi8_loop%=\n\t"

            "satoi8_end%=:\n\t"
            : "=&r"(ret)             // output ret, W/O random register
            : "r"(s1)               // input s1, R/W random register
            : "%ax"                 // modifying eAX
        );
    return ret;
}
// base 16 string to short
short satoi16(char const* volatile s)
{
    // make local variables volatile because the output with -O2 or
    //    -O3 was causing strings to become NULL for whatever reason
    // The s pointer is copied for the very same reason
    char const* volatile s1 = s;
    short volatile ret;
    asm volatile (   
            "xorw %%ax, %%ax\n\t"    // upper three bytes -> 0
            "xorw %0, %0\n\t"       // ret = 0

            "satoi16_loop%=:\n\t"     // while(*s1) {
            "movb (%1), %%al\n\t"
            "cmpw $0, %%ax\n\t"
            "jz satoi16_end%=\n\t"
            "shlw $4, %0\n\t"       //      ret <<= 4

            "cmpw $0x61, %%ax\n\t" //      if(*s1 > 'a') goto small_letters
            "jge satoi16_small_letters%=\n\t"

            "cmpw $0x41, %%ax\n\t" //      if(*s1 > 'A') goto big_letters
            "jge satoi16_big_letters%=\n\t"

            "subw $0x30, %%ax\n\t" //      eax = *s1 - '0'
            "satoi16_sum%=:\n\t"
            "addw %%ax, %0\n\t"    //      ret += eax
            "inc %1\n\t"            //      s1++
            "jmp satoi16_loop%=\n\t"

            "satoi16_big_letters%=:\n\t"
            "subw $0x37, %%ax\n\t" //      eax = *s1 - 'A'
            "jmp satoi16_sum%=\n\t"

            "satoi16_small_letters%=:\n\t"
            "subw $0x57, %%ax\n\t" //      eax = *s1 - 'a'
            "jmp satoi16_sum%=\n\t"
            "satoi16_end%=:\n\t"
            : "=&r"(ret)
            : "r"(s1)
            : "%ax"
        );
    return ret;
}

// base 10 atos
short satoi10(char const* volatile s)
{
    char const* s1 = s;
    short volatile ret;
    asm volatile(
            "xorw %%ax, %%ax\n\t"    // upper three bytes -> 0
            "xorw %0, %0\n\t"       // ret = 0

            "satoi10_loop%=:\n\t"      // while(*s1) {
            "movb (%1), %%al\n\t"
            "cmpw $0, %%ax\n\t"
            "jz satoi10_end%=\n\t"
            "movw %0, %%bx\n\t"        // ret *= 10
            "shlw $1, %%bx\n\t"
            "shlw $3, %0\n\t"
            "addw %%bx, %0\n\t"

            "subw $0x30, %%ax\n\t" //      eax = *s1 - '0'
            "satoi10_sum%=:\n\t"
            "addw %%ax, %0\n\t"    //      ret += eax
            "inc %1\n\t"            //      s1++
            "jmp satoi10_loop%=\n\t"

            "satoi10_end%=:\n\t"
            : "=&r"(ret)             // output ret, W/O random register
            : "r"(s1)               // input s1, R/W random register
            : "%eax", "%ebx"         // modifying eAX, ebx
        );
    return ret;
}



int main()
{
    char const*  s1 = "0F27F";
    char const*  s2 = "FF";
    char const*  s3 = "C0";
    char const*  s4 = "0C";

    char const*  s5 = "0123";
    char const*  s6 = "10";
    char const*  s7 = "777";
    char const* s12 = "0777777777777777";

    char const* s8 = "123";
    char const* s9 = "10";
    char const* s10 = "5";
    char const* s11 = "11";
    char const* s13 = "9999999999999999";

    printf("take into account return type sizes\n");
    printf("str -> matoi16(str) // should be equal in value\n");
    printf("%5s = %4X = %d (10)\n", s1, matoi16(s1), matoi16(s1));
    printf("%5s = %4X = %d (10)\n", s2, matoi16(s2), matoi16(s2));
    printf("%5s = %4X = %d (10)\n", s3, matoi16(s3), matoi16(s3));
    printf("%5s = %4X = %d (10)\n", s4, matoi16(s4), matoi16(s4));

    printf("str -> matoi8(str) // should be equal in value\n");
    printf("%5s = %4o = %d (10)\n", s5, matoi8(s5), matoi8(s5));
    printf("%5s = %4o = %d (10)\n", s6, matoi8(s6), matoi8(s6));
    printf("%5s = %4o = %d (10)\n", s7, matoi8(s7), matoi8(s7));
    printf("%5s = %4o = %d (10)\n", s12, matoi8(s12), matoi8(s12));

    printf("str -> matoi10(str) // should be equal in value\n");
    printf("%5s = %4d\n", s8, matoi10(s8));
    printf("%5s = %4d\n", s9, matoi10(s9));
    printf("%5s = %4d\n", s10, matoi10(s10));
    printf("%5s = %4d\n", s11, matoi10(s11));
    printf("%5s = %4d\n", s13, matoi10(s13));

    printf("str -> satoi8(str) // should be equal in value\n");
    printf("%5s = %4o = %d (10)\n", s5, satoi8(s5), satoi8(s5));
    printf("%5s = %4o = %d (10)\n", s6, satoi8(s6), satoi8(s6));
    printf("%5s = %4o = %d (10)\n", s7, satoi8(s7), satoi8(s7));
    printf("%5s = %4o = %d (10)\n", s12, satoi8(s12), satoi8(s12));

    printf("str -> satoi10(str) // should be equal in value\n");
    printf("%5s = %4d\n", s8, satoi10(s8));
    printf("%5s = %4d\n", s9, satoi10(s9));
    printf("%5s = %4d\n", s10, satoi10(s10));
    printf("%5s = %4d\n", s11, satoi10(s11));
    printf("%5s = %4d\n", s13, satoi10(s13));

    printf("str -> satoi16(str) // should be equal in value\n");
    printf("%5s = %4X = %d (10)\n", s1, satoi16(s1), satoi16(s1));
    printf("%5s = %4X = %d (10)\n", s2, satoi16(s2), satoi16(s2));
    printf("%5s = %4X = %d (10)\n", s3, satoi16(s3), satoi16(s3));
    printf("%5s = %4X = %d (10)\n", s4, satoi16(s4), satoi16(s4));

    return 0;
}
