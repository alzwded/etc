#include "model.hpp"
#include "mapModel.cpp"
#include <cstring>

namespace std {
template<>
struct less<const char*> {
    bool operator()(const char* a, const char* b) 
    {
        int l1 = strlen(a);
        int l2 = strlen(b);
        if(l1 != l2) return l1 < l2;
        int x = (strcmp(a, b) < 0);
        if(x != 0) {
            return x < 0;
        } else {
            return a < b;
        }
    }
};
}

mapModel(int, OfInt)
mapModel(char*, OfPChar)



element_s::element_s(void* _data)
{ 
    data = _data;
    dealloc = DEFAULT_DEALLOC;
}

element_s::element_s(void* _data, dealloc_t _dealloc)
{
    data = _data;
    dealloc = _dealloc;
}

element_s::~element_s()
{
    if(dealloc) dealloc(&data); 
}

void dealloc_free(void** who)
{
    if(*who != NULL) {
        free(*who);
        *who = NULL;
    }
}

queueModel::queueModel()
{
    lastError = 0;
    lastHandle = 1;
}

HANDLE queueModel::QueueCreate()
{
    if(lastHandle < MAX_HANDLE) {
        queues.insert(pair<HANDLE, queue<element_t> >(lastHandle, queue<element_t>()));
        lastError = 0;
        return lastHandle++;
    } else {
        lastHandle = 1;
        while(queues.find(lastHandle) != queues.end() && lastHandle < MAX_HANDLE) {
            lastHandle ++;
        }
        if(lastHandle < MAX_HANDLE) {
            queues.insert(pair<HANDLE, queue<element_t> >(lastHandle, queue<element_t>()));
            lastError = 0;
            return lastHandle++;
        } else {
            // TODO implement error codes
            return NULL_HANDLE;
        }
    }
}

bool queueModel::QueueFront(const HANDLE _handle, void** ret)
{
    bool is;
    if(!QueueIsEmpty(_handle, &is) || is) {
        return false;
    }
    *ret = (queues[_handle].front().data);
    return true;
}

bool queueModel::QueueBack(const HANDLE _handle, void** ret)
{
    bool is;
    if(!QueueIsEmpty(_handle, &is) || is) {
        return false;
    }
    *ret = (queues[_handle].back().data);
    return true;
}

bool queueModel::QueuePop(const HANDLE _handle)
{
    bool ret;
    if(!QueueIsEmpty(_handle, &ret) || ret) {
        return false;
    }
    queues[_handle].pop();
    return true;
}

bool queueModel::QueuePush(const HANDLE _handle, const void* data)
{
    if(queues.find(_handle) == queues.end()) {
        return false;
    }
    queues[_handle].push(element_t(const_cast<void*>(data)));
    return true;
}

bool queueModel::QueuePush(const HANDLE _handle, const void* data, dealloc_t dealloc)
{
    if(queues.find(_handle) == queues.end()) {
        return false;
    }
    queues[_handle].push(element_t(const_cast<void*>(data), dealloc));
    return true;
}

bool queueModel::QueueIsEmpty(const HANDLE _handle, bool* ret)
{
    if(queues.find(_handle) == queues.end()) {
        return false;
    }
    *ret = (queues[_handle].size() == 0);
    return true;
}

void queueModel::QueueDelete(const HANDLE _handle)
{
    map<HANDLE, queue<element_t> >::iterator i;
    if((i = queues.find(_handle)) != queues.end()) {
        queues.erase(i);
    }
}

vectorModel::vectorModel()
{
    lastError = 0;
    lastHandle = 1;
}

HANDLE vectorModel::VectorCreate()
{
    if(lastHandle < MAX_HANDLE) {
        vectors.insert(pair<HANDLE, vector<element_t> >(lastHandle, vector<element_t>()));
        lastError = 0;
        return lastHandle++;
    } else {
        lastHandle = 1;
        while(vectors.find(lastHandle) != vectors.end() && lastHandle < MAX_HANDLE) {
            lastHandle ++;
        }
        if(lastHandle < MAX_HANDLE) {
            vectors.insert(pair<HANDLE, vector<element_t> >(lastHandle, vector<element_t>()));
            lastError = 0;
            return lastHandle++;
        } else {
            // TODO implement error codes
            return NULL_HANDLE;
        }
    }
}

HANDLE vectorModel::VectorCreate(const int initialSize)
{
    HANDLE ret = VectorCreate();
    if(ret != NULL_HANDLE) {
        VectorResize(ret, initialSize);
    }
    return ret;
    /*
    if(lastHandle < MAX_HANDLE) {
        vectors.insert(pair<HANDLE, vector<element_t> >(lastHandle, vector<element_t>(initialSize)));
        lastError = 0;
        return lastHandle++;
    } else {
        lastHandle = 1;
        while(vectors.find(lastHandle) != vectors.end() && lastHandle < MAX_HANDLE) {
            lastHandle ++;
        }
        if(lastHandle < MAX_HANDLE) {
            vectors.insert(pair<HANDLE, vector<element_t> >(lastHandle, vector<element_t>(initialSize)));
            lastError = 0;
            return lastHandle++;
        } else {
            // TODO implement error codes
            return NULL_HANDLE;
        }
    }*/
}

void vectorModel::VectorDelete(const HANDLE _handle)
{
    map<HANDLE, vector<element_t> >::iterator i;
    if((i = vectors.find(_handle)) != vectors.end()) {
        vectors.erase(i);
    }
}

bool vectorModel::VectorGet(const HANDLE _handle, const int index, void** ret)
{
    if(vectors.find(_handle) == vectors.end()) {
        return false;
    }
    if(index >= vectors[_handle].size()) {
        return false;
    }
    *ret = vectors[_handle][index].data;
    return true;
}

bool vectorModel::VectorSet(const HANDLE _handle, const int index, void* data)
{
    if(vectors.find(_handle) == vectors.end()) {
        return false;
    }
    try {
        vectors[_handle][index] = element_t(data);
        return true;
    } catch(...) {
        return false;
    }
}

bool vectorModel::VectorSet(const HANDLE _handle, const int index, void* data, dealloc_t dealloc)
{
    if(vectors.find(_handle) == vectors.end()) {
        return false;
    }
    try {
        vectors[_handle][index] = element_t(data, dealloc);
        return true;
    } catch(...) {
        return false;
    }
}

bool vectorModel::VectorPushBack(const HANDLE _handle, void* data)
{
    if(vectors.find(_handle) == vectors.end()) {
        return false;
    }
    vectors[_handle].push_back(element_t(data));
    return true;
}

bool vectorModel::VectorPushBack(const HANDLE _handle, void* data, dealloc_t dealloc)
{
    if(vectors.find(_handle) == vectors.end()) {
        return false;
    }
    vectors[_handle].push_back(element_t(data, dealloc));
    return true;
}

bool vectorModel::VectorPopBack(const HANDLE _handle)
{
    if(vectors.find(_handle) == vectors.end()) {
        return false;
    }
    vectors[_handle].pop_back();
    return true;
}

bool vectorModel::VectorInsert(const HANDLE _handle, const int position, void* data)
{
    if(vectors.find(_handle) == vectors.end()) {
        return false;
    }
    vector<element_t>::iterator pos = vectors[_handle].begin() + position;
    vectors[_handle].insert(pos, element_t(data));
    return true;
}

bool vectorModel::VectorInsert(const HANDLE _handle, const int position, void* data, dealloc_t dealloc)
{
    if(vectors.find(_handle) == vectors.end()) {
        return false;
    }
    vector<element_t>::iterator pos = vectors[_handle].begin() + position;
    vectors[_handle].insert(pos, element_t(data, dealloc));
    return true;
}

bool vectorModel::VectorInsert(const HANDLE _handle, const int position, const int size, void* data)
{
    if(vectors.find(_handle) == vectors.end()) {
        return false;
    }
    vector<element_t>::iterator pos = vectors[_handle].begin() + position;
    vectors[_handle].insert(pos, size, element_t(data));
    return true;
}

bool vectorModel::VectorInsert(const HANDLE _handle, const int position, const int size, void* data, dealloc_t dealloc)
{
    if(vectors.find(_handle) == vectors.end()) {
        return false;
    }
    vector<element_t>::iterator pos = vectors[_handle].begin() + position;
    vectors[_handle].insert(pos, size, element_t(data, dealloc));
    return true;
}

bool vectorModel::VectorErase(const HANDLE _handle, const int position)
{
    if(vectors.find(_handle) == vectors.end()) {
        return false;
    }
    vectors[_handle].erase(vectors[_handle].begin() + position);
    return true;
}

bool vectorModel::VectorErase(const HANDLE _handle, const int start, const int end)
{
    if(vectors.find(_handle) == vectors.end()) {
        return false;
    }
    vectors[_handle].erase(vectors[_handle].begin() + start, vectors[_handle].begin() + end);
    return true;
}

bool vectorModel::VectorSize(const HANDLE _handle, int* size)
{
    if(vectors.find(_handle) == vectors.end()) {
        return false;
    }
    *size = vectors[_handle].size();
    return true;
}

bool vectorModel::VectorEmpty(const HANDLE _handle, bool* b)
{
    if(vectors.find(_handle) == vectors.end()) {
        return false;
    }
    *b = vectors[_handle].empty();
    return true;
}

bool vectorModel::VectorReserve(const HANDLE _handle, const int size)
{
    if(vectors.find(_handle) == vectors.end()) {
        return false;
    }
    vectors[_handle].reserve(size);
    return true;
}

bool vectorModel::VectorResize(const HANDLE _handle, const int size)
{
    if(vectors.find(_handle) == vectors.end()) {
        return false;
    }
    vectors[_handle].resize(size);
    return true;
}

bool vectorModel::VectorFront(const HANDLE _handle, void** ret)
{
    if(vectors.find(_handle) == vectors.end()) {
        return false;
    }
    if(vectors[_handle].size() <= 0) {
        return false;
    }
    *ret = vectors[_handle].front().data;
    return true;
}

bool vectorModel::VectorBack(const HANDLE _handle, void** ret)
{
    if(vectors.find(_handle) == vectors.end()) {
        return false;
    }
    if(vectors[_handle].size() <= 0) {
        return false;
    }
    *ret = vectors[_handle].back().data;
    return true;
}

