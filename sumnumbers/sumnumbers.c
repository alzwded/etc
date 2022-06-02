/*
Copyright 2022 Vlad Mesco

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <math.h>

#define OPTSTRING "he"

char** snumbers = NULL;
int nnumbers = 0;

__attribute__((noreturn)) void usage(const char* argv0)
{
    if(!argv0) argv0 = "sumnumbers";
    printf("Usage: %s [-%s] [numbers....]\n", argv0, OPTSTRING);
    printf("    -h      prints this message\n");
    printf("    -e      returns sum as error code instead of printing to stdout\n");
    printf("            this rounds to the nearest int\n");
    printf("    If numbers are supplied as args, it prints the sum and exits\n");
    printf("    Else, reads numbers from stdin, prints the sum and exits\n");
    printf("    This uses floating point (double) internally\n");
    exit(1);
}

int from_stdin(double* n)
{
    while(!(1 == scanf("%lf", n))) {
        if(feof(stdin)) return 0;
        if(ferror(stdin)) return 0;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
        (void) scanf("%*s");
#pragma GCC diagnostic pop
    }
    return (1 == scanf("%lf", n));
}

int from_args(double* n)
{
    if(nnumbers <= 0) return 0;
    *n = atof(*snumbers);
    nnumbers--;
    snumbers++;
    return 1;
}

double doSum(int (*fn)(double*))
{
    double s = 0.0, n = 0.0;
    while(fn(&n)) {
        //fprintf(stderr, "%lf += %lf\n", s, n);
        s += n;
    }
    return s;
}

int main(int argc, char* argv[])
{
    int opt, return_sum = 0;
    double sum = 0.0;
    while((opt = getopt(argc, argv, OPTSTRING)) != -1) {
        switch(opt) {
            case 'e':
                return_sum = 1;
                break;
            default:
            case 'h':
                usage(argv[0]);
        }
    }

    snumbers = argv + optind;
    nnumbers = argc - optind;


    if(optind >= argc) {
        sum = doSum(&from_stdin);
    } else {
        sum = doSum(&from_args);
    }

    if(!return_sum) {
        printf("%.15lg\n", sum);
        return 0;
    } else {
        return (int)round(sum);
    }
}
