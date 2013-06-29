#ifndef STL_STRINGMAP_H
#define STL_STRINGMAP_H

#include "declarations.h"

#ifdef __cplusplus
extern "C" {
#endif
    HANDLE stringmap_create();
    int stringmap_get(const HANDLE _handle, const char* key, void** ret);
    int stringmap_set(const HANDLE _handle, const char* key, const void* data);
    int stringmap_setd(const HANDLE _handle, const char* key, const void* data, dealloc_t dealloc);
    int stringmap_remove(const HANDLE _handle, const char* key);
    int stringmap_haskey(const HANDLE _handle, const char* key, int* ret);
    void stringmap_delete(HANDLE _handle);
#ifdef __cplusplus
}
#endif

#endif
