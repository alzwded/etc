#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fft.h"

#define FFT(N, X) fft(N, (float(*)[2])X)
#define IFFT(N, X) ifft(N, (float(*)[2])X)

int main(int argc, char* argv[])
{
#define NSAMPLES 512
#define PI 3.14159f
    float x[NSAMPLES * 2];
    size_t i;

    float fqv = 430.66936f;

    if(argc > 1) fqv = atof(argv[1]);

    printf("transforming a sine wave of frequency %f\n", fqv);

    if(argc > 2) printf("x:\n");
    for(i = 0; i < NSAMPLES; ++i) {
        float arg = 2.f * PI * (float)i * (fqv / 44100.f);
        x[2 * i] = sinf(arg);
        x[2 * i + 1] = 0.f;
        if(argc > 2) printf("%10f + i%10f\n", x[2 * i], x[2 * i + 1]);
    }

    FFT(NSAMPLES, x);

    if(argc > 2) printf("======================\n");
    printf("X:\n");
    for(i = 0; i < NSAMPLES / 2; ++i) {
        if(argc > 2) printf("%10f + i%10f\n", x[2 * i], x[2 * i + 1]);
        printf("%10.2fHz: %10f\n", (float)i * 44100.f / NSAMPLES, sqrt(x[2 * i] * x[2 * i] + x[2 * i + 1] * x[2 * i + 1]));
    }

    if(argc > 2) {
        IFFT(NSAMPLES, x);
        printf("======================\n");
        printf("x:\n");
        for(i = 0; i < NSAMPLES; ++i) {
            printf("%10f + i%10f\n", x[2 * i], x[2 * i + 1]);
        }
    }

    return 0;
}
