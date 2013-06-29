#ifndef STL_MAP_H
#define STL_MAP_H

#include "declarations.h"

#ifdef __cplusplus
extern "C" {
#endif
    HANDLE map_create();
    int map_get(const HANDLE _handle, const int key, void** ret);
    // uses default deallocator, which calls free on the pointer
    int map_set(const HANDLE _handle, const int key, const void* data);
    // pass NO_DEALLOC to not free any memory from data
    // you can pass your own dealloc function
    int map_setd(const HANDLE _handle, const int key, const void* data, dealloc_t dealloc);
    int map_remove(const HANDLE _handle, const int key);
    int map_haskey(const HANDLE _handle, const int key, int* ret);
    void map_delete(HANDLE _handle);
#ifdef __cplusplus
}
#endif
#endif
