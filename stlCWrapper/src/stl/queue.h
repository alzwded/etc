#ifndef STL_QUEUE_H
#define STL_QUEUE_H

#include "declarations.h"

#ifdef __cplusplus
extern "C" {
#endif
    HANDLE queue_create();
    int queue_front(const HANDLE _handle, void** ret);
    int queue_back(const HANDLE _handle, void** ret);
    int queue_pop(const HANDLE _handle);
    int queue_push(const HANDLE _handle, const void* data);
    int queue_pushd(const HANDLE _handle, const void* data, dealloc_t dealloc);
    int queue_isempty(const HANDLE _handle, int* ret);
    void queue_delete(HANDLE _handle);
#ifdef __cplusplus
}
#endif

#endif
