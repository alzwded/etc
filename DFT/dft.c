// SLOW Discrete Fourier Transform

#include <complex.h>
#include <stdio.h>
#include <tgmath.h>
#include <stdlib.h>

#define pi (acos(-1.f))

void DFT(size_t N, _Complex float* x, size_t NF, _Complex float* X)
{
    size_t sample, k;
    _Complex float w;

    for(k = 0; k < NF; ++k) {
        X[k] = 0;
        float arg = (-2.f * pi * (float)k) / (float)N;
        for(sample = 0; sample < N; ++sample) {
            w = cos((float)sample * arg) + I * sin((float)sample * arg);
            X[k] += x[sample] * w;
        }
    }
}

void IDFT(size_t NF, _Complex float* X, size_t N, _Complex float* x)
{
#if 0
    _Complex float* X1 = (_Complex float*)malloc(sizeof(_Complex float) * NF);
    size_t k;

    // swap real and imag parts
    for(k = 0; k < NF; ++k) {
        X1[k] = cimagf(X[k]) + I * crealf(X[k]);
    }

    // apply DFT transform
    DFT(NF, X1, N, x);

    // swap real and imag parts again
    for(k = 0; k < N; ++k) {
        x[k] = cimagf(x[k]) + I * crealf(x[k]);
        x[k] /= N;
    }

    free(X1);
#else
    // this also works! :D but rather poorly :(

    size_t sample, k;
    _Complex float w;

    for(sample = 0; sample < N; ++sample) {
        x[sample] = 0;
        float arg = (2.f * pi * (float)sample) / (float)NF;
        for(k = 0; k < NF; ++k) {
            w = cos((float)k * arg) + I * sin((float)k * arg);
            x[sample] += X[k] * w;
        }
        x[sample] /= (float)N;
    }
#endif
}

#ifdef TEST
int main(int argc, char* argv[])
{
#define NSAMPLES 512
//#define NFREQS NSAMPLES / 2
#define NFREQS NSAMPLES
    _Complex float x[NSAMPLES];
    _Complex float X[NFREQS];
    size_t i;

    float fqv = 440.f;

    if(argc > 1) fqv = atof(argv[1]);

    printf("transforming a sine wave of frequency %f\n", fqv);

    if(argc > 2) printf("x:\n");
    for(i = 0; i < NSAMPLES; ++i) {
        float arg = 2.f * pi * (float)i * (fqv / 44100.f);
        x[i] = sin(arg) + I * 0.f;
        if(argc > 2) printf("%10f + i%10f\n", creal(x[i]), cimag(x[i]));
    }

    DFT(NSAMPLES, x, NFREQS, X);

    if(argc > 2) printf("======================\n");
    printf("X:\n");
    for(i = 0; i < NFREQS; ++i) {
        if(argc > 2) printf("%10f + i%10f\n", crealf(X[i]), cimagf(X[i]));
        printf("%10fHz: %10f\n", (float)i * 44100.f / NSAMPLES, cabsf(X[i]));
    }

    if(argc > 2) {
        IDFT(NFREQS, X, NSAMPLES, x);
        printf("======================\n");
        printf("x:\n");
        for(i = 0; i < NSAMPLES; ++i) {
            printf("%10f + i%10f\n", crealf(x[i]), cimagf(x[i]));
        }
        for(i = 0; i < NSAMPLES; ++i) {
            printf("%10f\n", cabsf(x[i]));
        }
    }

    return 0;
}
#endif
