// SLOW Discrete Fourier Transform

#include <complex>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <vector>

#define pi (std::acos(-1.0))

#if 0
# define NSAMPLES 512
# define SAMPFREQ 44100.f
#else
# define NSAMPLES 40
# define SAMPFREQ 40.0
#endif

void DFT(std::vector<std::complex<double>> const& x, std::vector<std::complex<double>>& X)
{
    X.clear();
    X.resize(x.size());

    for(size_t k = 0; k < X.size(); ++k)
    {
        X[k] = std::polar(0.0, 0.0);
        double arg = (-2.0 * pi * (double)k) / x.size();
        for(size_t sample = 0; sample < x.size(); ++sample)
        {
            X[k] += x[sample] * std::complex<double>(
                        std::cos((double)sample * arg),
                        std::sin((double)sample * arg));
        }
    }
}

void IDFT(std::vector<std::complex<double>> const& X, std::vector<std::complex<double>>& x)
{
    x.clear();
    x.resize(X.size());

    for(size_t sample = 0; sample < x.size(); ++sample)
    {
        x[sample] = std::polar(0.0, 0.0);
        double arg = (2.0 * pi * (double)sample) / (double)X.size();
        for(size_t k = 0; k < X.size(); ++k)
        {
            x[sample] += X[k] * std::complex<double>(
                        std::cos((double)k * arg),
                        std::sin((double)k * arg));
        }
        x[sample] /= (double)x.size();
    }
}

#ifdef TEST
int main(int argc, char* argv[])
{
//#define NSAMPLES 512
//#define NFREQS NSAMPLES / 2
#define NFREQS NSAMPLES
    std::vector<std::complex<double>> x, X;
    x.resize(NSAMPLES);
    X.resize(NFREQS);
    size_t i;

    double fqv = 13.0;

    if(argc > 1) fqv = atof(argv[1]);

    printf("transforming a sine wave of frequency %f + sin(3Hz)\n", fqv);

    if(argc > 2) printf("x:\n");
    for(i = 0; i < NSAMPLES; ++i) {
        float arg = 2.f * pi * (double)i * (fqv / SAMPFREQ);
        x[i] = std::complex<double>(sin(arg), 0.0);
#if 1
        x[i] += std::complex<double>(sin(2.f * pi * i * (3.f/SAMPFREQ)), 0.0);
#endif
        if(argc > 2) printf("%10f + i%10f\n", x[i].real(), x[i].imag());
    }

    DFT(x, X);

    if(argc > 2) printf("======================\n");
    printf("X:\n");
    for(i = 0; i < NFREQS / 2.f; ++i) {
        if(argc > 2) printf("%10f + i%10f\n", X[i].real(), X[i].imag());
        printf("%10.2fHz: %10f\n", (double)i * SAMPFREQ / NSAMPLES, std::abs(X[i]));
    }

    if(argc > 2) {
        IDFT(X, x);
        printf("======================\n");
        printf("x:\n");
        for(i = 0; i < NSAMPLES; ++i) {
            printf("%10f + i%10f\n", x[i].real(), x[i].imag());
        }
        for(i = 0; i < NSAMPLES; ++i) {
            //printf("%10f\n", std::abs(x[i]));
            printf("%10f\n", x[i].real());
        }
    }

    return 0;
}
#endif
