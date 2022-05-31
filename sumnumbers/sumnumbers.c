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
    printf("    If numbers are supplied as args, it prints the sum and exits\n");
    printf("    Else, reads numbers from stdin, prints the sum and exits\n");
    printf("    This uses floating point (double) internally\n");
    exit(1);
}

int from_stdin(double* n)
{
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
        printf("%lg\n", sum);
        return 0;
    } else {
        return (int)round(sum);
    }
}
