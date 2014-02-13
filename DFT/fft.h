#ifndef FFT_H
#define FFT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// in-place radix-2 DIT FastFourierTransform and InverseFFT
// the length of the input must be a power of two
// float (*)[2] should correspond to either _Complex float* (C99) or 
//     std::complex<float>* i.e. std::vector<std::complex<float>>.data() (C++)
// and, of course, float (*)[2] should correspond to float[2 * N] or
//     float [N][2] i.e. typedef float C_t[2]; C_t v[N] in classic C89
void fft(size_t N, float (*X)[2]);
void ifft(size_t N, float (*X)[2]);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
