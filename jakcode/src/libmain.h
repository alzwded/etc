#ifndef JAKCODE_H
#define JAKCODE_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
// the functions close the two streams before returning
void jakcode_stream_encode(FILE* fin, FILE* fout);
void jakcode_stream_decode(FILE* fin, FILE* fout);

// you need to manually do free(out) after you're done with the data
// usage: jakcode_mem_??code(inputData, sizeOfInputData, &outputData, &sizeOfOutputData)
void jakcode_mem_encode(const unsigned char* in, int sin, unsigned char** out, int* sout);
void jakcode_mem_decode(const unsigned char* in, int sin, unsigned char** out, int* sout);
#ifdef __cplusplus
}
#endif

#endif
