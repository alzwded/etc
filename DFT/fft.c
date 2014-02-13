#include <complex.h>
#include <tgmath.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static inline size_t mylog(size_t n)
{
    int ret = 0;
#ifndef NDEBUG
    size_t norig = n;
#endif
    while(n > 1) {
        n >>= 1;
        ret++;
    }
    assert((1 << ret) == norig);
    return ret;
}

#define PI (3.14159265f)

static inline void fft_brs(size_t N, _Complex float* X)
{
    size_t i, j, k;
    _Complex float T;

    // bit reversal sorting
    for(i = 1, j = N/2; i <= N - 2; ++i) {
        if(i < j) {
            T = X[j];
            X[j] = X[i];
            X[i] = T;
        }

        k = N / 2;
        while(k <= j) {
            j -= k;
            k /= 2;
        }
        j += k;
    }
}

static inline void fft_dnq(size_t N, _Complex float* X)
{
    size_t logn = mylog(N);
    size_t j;
    size_t i, k, stride, m;
    _Complex float T, w, s;

    // compute logn stages of divide and conquer
    for(k = 1; k <= logn; ++k) {
        stride = 1 << k;
        m = stride / 2;
        w = 1.0f;
        s = cos(PI / (float)m) - I * sin(PI / (float)m);
        for(j = 1; j <= m; ++j) {
            for(i = j - 1; i < N; i += stride) {
                T = X[i + m] * w;
                X[i + m] = X[i] - T;
                X[i] = X[i] + T;
            }
            w = w * s;
        }
    }
}

void fft(size_t N, _Complex float* X)
{
    // bit reversal sorting
    fft_brs(N, X);
    // log(N) stages of summing partial results
    fft_dnq(N, X);
}

static inline void ifft_changeSign(size_t N, _Complex float* x)
{
    size_t i;
    for(i = 0; i < N; ++i) {
        x[i] = crealf(x[i]) - I * cimagf(x[i]);
    }
}

void ifft(size_t N, _Complex float* x)
{
    size_t i;
    // change sign of imaginary part
    ifft_changeSign(N, x);

    // apply forward fft
    fft(N, x);
    
    // change sign again
    ifft_changeSign(N, x);
    // and scale
    for(i = 0; i < N; ++i) {
        x[i] /= (float)N;
    }
}

#ifdef TEST
int main(int argc, char* argv[])
{
#define NSAMPLES 512
    _Complex float x[NSAMPLES];
    size_t i;

    float fqv = 430.66936f;

    if(argc > 1) fqv = atof(argv[1]);

    printf("transforming a sine wave of frequency %f\n", fqv);

    if(argc > 2) printf("x:\n");
    for(i = 0; i < NSAMPLES; ++i) {
        float arg = 2.f * PI * (float)i * (fqv / 44100.f);
        x[i] = sin(arg) + I * 0.f;
        if(argc > 2) printf("%10f + i%10f\n", creal(x[i]), cimag(x[i]));
    }

    fft(NSAMPLES, x);

    if(argc > 2) printf("======================\n");
    printf("X:\n");
    for(i = 0; i < NSAMPLES / 2; ++i) {
        if(argc > 2) printf("%10f + i%10f\n", crealf(x[i]), cimagf(x[i]));
        printf("%10.2fHz: %10f\n", (float)i * 44100.f / NSAMPLES, cabsf(x[i]));
    }

    if(argc > 2) {
        ifft(NSAMPLES, x);
        printf("======================\n");
        printf("x:\n");
        for(i = 0; i < NSAMPLES; ++i) {
            printf("%10f + i%10f\n", crealf(x[i]), cimagf(x[i]));
        }
    }

    return 0;
}
#endif
