#ifndef STL_VECTOR_H
#define STL_VECTOR_H

#include "declarations.h"

#ifdef __cplusplus
extern "C" {
#endif
    HANDLE vector_create();
    HANDLE vector_create_reserve(const int size);
    void vector_delete(HANDLE _handle);
    int vector_get(const HANDLE _handle, const int index, void** ret);
    int vector_set(const HANDLE _handle, const int index, void* data);
    int vector_setd(const HANDLE _handle, const int index, void* data, dealloc_t dealloc);
    int vector_push_back(const HANDLE _handle, void* data);
    int vector_push_backd(const HANDLE _handle, void* data, dealloc_t dealloc);
    int vector_pop_back(const HANDLE _handle);
    int vector_insert(const HANDLE _handle, const int position, void* data);
    int vector_insertd(const HANDLE _handle, const int position, void* data, dealloc_t dealloc);
    // not functional -- use NO_DEALLOC. if a dealloc is used, it will crash
    int vector_insert_range(const HANDLE _handle, const int position, const int size, void* data);
    int vector_insert_ranged(const HANDLE _handle, const int position, const int size, void* data, dealloc_t dealloc);
    int vector_erase(const HANDLE _handle, const int position);
    int vector_erase_range(const HANDLE _handle, const int start, const int end);
    int vector_size(const HANDLE _handle, int* size);
    int vector_empty(const HANDLE _handle, int* b);
    int vector_reserve(const HANDLE _handle, const int size);
    int vector_resize(const HANDLE _handle, const int size);
    int vector_front(const HANDLE _handle, void** ret);
    int vector_back(const HANDLE _handle, void** ret);
#ifdef __cplusplus
}
#endif

#endif
