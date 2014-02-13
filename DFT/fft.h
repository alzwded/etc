#ifndef FFT_H
#define FFT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// float (*)[2] should correspond to either _Complex float* or 
//     std::complex<float>* i.e. std::vector<std::complex<float>>.data()
// and, of course, float (*)[2] should correspond to float[2 * N] or
//     float [N][2] in classic C89
void fft(size_t N, float (*X)[2]);
void ifft(size_t N, float (*X)[2]);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
