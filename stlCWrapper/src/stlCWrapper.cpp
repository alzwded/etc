#include "stl/stlCWrapper.h"
#include "model.hpp"

HANDLE map_create()
{
    return mapModelOfInt::getModel().MapCreate();
}

int map_get(const HANDLE _handle, const int key, void** ret)
{
    return mapModelOfInt::getModel().MapAccess(_handle, key, ret);
}

int map_set(const HANDLE _handle, const int key, const void* data)
{
    return mapModelOfInt::getModel().MapInsert(_handle, key, data);
}

int map_setd(const HANDLE _handle, const int key, const void* data, dealloc_t dealloc)
{
    return mapModelOfInt::getModel().MapInsert(_handle, key, data, dealloc);
}

int map_remove(const HANDLE _handle, const int key)
{
    return mapModelOfInt::getModel().MapRemove(_handle, key);
}

int map_haskey(const HANDLE _handle, const int key, int* ret)
{
    bool bRet;
    bool magicValue = mapModelOfInt::getModel().MapHasKey(_handle, key, &bRet);
    if(!magicValue) return 0;
    *ret = bRet;
    return 1;
}

void map_delete(HANDLE _handle)
{
    mapModelOfInt::getModel().MapDelete(_handle);
}

HANDLE stringmap_create()
{
    return mapModelOfPChar::getModel().MapCreate();
}

int stringmap_get(const HANDLE _handle, const char* key, void** ret)
{
    if(key == NULL) {
        return 0;
    }
    return mapModelOfPChar::getModel().MapAccess(_handle, key, ret);
}

int stringmap_set(const HANDLE _handle, const char* key, const void* data)
{
    if(key == NULL) {
        return 0;
    }
    return mapModelOfPChar::getModel().MapInsert(_handle, key, data);
}

int stringmap_setd(const HANDLE _handle, const char* key, const void* data, dealloc_t dealloc)
{
    if(key == NULL) {
        return 0;
    }
    return mapModelOfPChar::getModel().MapInsert(_handle, key, data, dealloc);
}

int stringmap_remove(const HANDLE _handle, const char* key)
{
    if(key == NULL) {
        return 0;
    }
    return mapModelOfPChar::getModel().MapRemove(_handle, key);
}

int stringmap_haskey(const HANDLE _handle, const char* key, int* ret)
{
    if(key == NULL) {
        return 0;
    }
    {
        bool bRet;
        bool magicValue = mapModelOfPChar::getModel().MapHasKey(_handle, key, &bRet);
        if(!magicValue) return 0;
        *ret = bRet;
        return 1;
    }
}

void stringmap_delete(HANDLE _handle)
{
    mapModelOfPChar::getModel().MapDelete(_handle);
}

HANDLE queue_create()
{
    return queueModel::getModel().QueueCreate();
}

int queue_front(const HANDLE _handle, void** ret)
{
    return queueModel::getModel().QueueFront(_handle, ret);
}

int queue_back(const HANDLE _handle, void** ret)
{
    return queueModel::getModel().QueueBack(_handle, ret);
}

int queue_pop(const HANDLE _handle)
{
    return queueModel::getModel().QueuePop(_handle);
}

int queue_push(const HANDLE _handle, const void* data)
{
    return queueModel::getModel().QueuePush(_handle, data);
}

int queue_pushd(const HANDLE _handle, const void* data, dealloc_t dealloc)
{
    return queueModel::getModel().QueuePush(_handle, data, dealloc);
}

int queue_isempty(const HANDLE _handle, int* ret)
{
    bool b;
    if(queueModel::getModel().QueueIsEmpty(_handle, &b)) {
        if(b) {
            *ret = 1;
        } else {
            *ret = 0;
        }
        return true;
    } else {
        return false;
    }
}

void queue_delete(HANDLE _handle)
{
    queueModel::getModel().QueueDelete(_handle);
}

HANDLE vector_create()
{
    return vectorModel::getModel().VectorCreate();
}

HANDLE vector_create_reserve(const int size) 
{
    return vectorModel::getModel().VectorCreate(size);
}

void vector_delete(HANDLE _handle) 
{
    vectorModel::getModel().VectorDelete(_handle);
}

int vector_get(const HANDLE _handle, const int index, void** ret)
{
    return vectorModel::getModel().VectorGet(_handle, index, ret);
}

int vector_set(const HANDLE _handle, const int index, void* data)
{
    return vectorModel::getModel().VectorSet(_handle, index, data);
}

int vector_setd(const HANDLE _handle, const int index, void* data, dealloc_t dealloc)
{
    return vectorModel::getModel().VectorSet(_handle, index, data, dealloc);
}

int vector_push_back(const HANDLE _handle, void* data)
{
    return vectorModel::getModel().VectorPushBack(_handle, data);
}

int vector_push_backd(const HANDLE _handle, void* data, dealloc_t dealloc)
{
    return vectorModel::getModel().VectorPushBack(_handle, data, dealloc);
}

int vector_pop_back(const HANDLE _handle)
{
    return vectorModel::getModel().VectorPopBack(_handle);
}

int vector_insert(const HANDLE _handle, const int position, void* data)
{
    return vectorModel::getModel().VectorInsert(_handle, position, data);
}

int vector_insertd(const HANDLE _handle, const int position, void* data, dealloc_t dealloc)
{
    return vectorModel::getModel().VectorInsert(_handle, position, data, dealloc);
}

int vector_insert_range(const HANDLE _handle, const int position, const int size, void* data)
{
    return vectorModel::getModel().VectorInsert(_handle, position, size, data);
}

int vector_insert_ranged(const HANDLE _handle, const int position, const int size, void* data, dealloc_t dealloc)
{
    return vectorModel::getModel().VectorInsert(_handle, position, size, data, dealloc);
}

int vector_erase(const HANDLE _handle, const int position)
{
    return vectorModel::getModel().VectorErase(_handle, position);
}

int vector_erase_range(const HANDLE _handle, const int start, const int end)
{
    return vectorModel::getModel().VectorErase(_handle, start, end);
}

int vector_size(const HANDLE _handle, int* size)
{
    return vectorModel::getModel().VectorSize(_handle, size);
}

int vector_empty(const HANDLE _handle, int* b)
{
    bool bb;
    bool awhat = vectorModel::getModel().VectorEmpty(_handle, &bb);
    *b = (int)bb;
    return awhat;
}

int vector_reserve(const HANDLE _handle, const int size)
{
    return vectorModel::getModel().VectorReserve(_handle, size);
}

int vector_resize(const HANDLE _handle, const int size)
{
    return vectorModel::getModel().VectorResize(_handle, size);
}

int vector_front(const HANDLE _handle, void** ret)
{
    return vectorModel::getModel().VectorFront(_handle, ret);
}

int vector_back(const HANDLE _handle, void** ret)
{
    return vectorModel::getModel().VectorBack(_handle, ret);
}

