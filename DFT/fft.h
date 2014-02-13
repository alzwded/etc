#ifndef FFT_H
#define FFT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void fft(size_t N, float (*X)[2]);
void ifft(size_t N, float (*X)[2]);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
