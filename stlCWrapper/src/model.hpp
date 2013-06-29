#ifndef MODEL_HPP
#define MODEL_HPP

#include <map>
#include <queue>
#include <vector>
#include <cstdlib>
#include "stl/declarations.h"
#include "mapModel.hpp"

using namespace std;

// default deallocator
void dealloc_free(void** who);
typedef struct element_s element_t;
typedef struct element_s {
    // actual data stored
    void* data;
    // deallocator. Can be assigned to. By default, calls free on data if
    //      data is not null
    //      use NO_DEALLOC to not free anything
    dealloc_t dealloc;

    explicit element_s(void* _data = NULL); 
    explicit element_s(void* _data, dealloc_t _dealloc); 
    element_s(const element_t& other)
    {
        data = other.data;
        dealloc = other.dealloc;
        // nullify the deallocator because this wants to keep the data
        //      other will be destroyed
        const_cast<element_t*>(&other)->dealloc = NO_DEALLOC;
    }
    element_s& operator=(element_t other)
    {
        if(data) {
            if(dealloc) {
                dealloc(&data);
            }
        }
        data = other.data;
        dealloc = other.dealloc;
        // nullify the deallocator because this wants to keep the data
        //      other will be destroyed
        other.dealloc = NO_DEALLOC;
        return *this;
    }
    ~element_s();
} element_t;


// STL map mapModel
// templatization done with defines because I couldn't figure out how
// to link the template classes to the extern C functions
mapModelHeader(int, OfInt)
mapModelHeader(char*, OfPChar)

class queueModel {
private:
    map<HANDLE, queue<element_t> > queues;
    HANDLE lastHandle;
    int lastError;
    queueModel();
public:
    static queueModel& getModel()
    {
        static queueModel* theModel = new queueModel();
        return *theModel;
    }
    HANDLE QueueCreate();
    bool QueueFront(const HANDLE _handle, void** ret);
    bool QueueBack(const HANDLE _handle, void** ret);
    bool QueuePop(const HANDLE _handle);
    bool QueuePush(const HANDLE _handle, const void* data);
    bool QueuePush(const HANDLE _handle, const void* data, dealloc_t dealloc);
    bool QueueIsEmpty(const HANDLE _handle, bool* ret);
    void QueueDelete(const HANDLE _handle);

    int getLastError() { int ret = lastError; return ret; }
};

class vectorModel {
    private:
    map<HANDLE, vector<element_t> > vectors;
    HANDLE lastHandle;
    int lastError;
    vectorModel();
public:
    static vectorModel& getModel()
    {
        static vectorModel* theModel = new vectorModel();
        return *theModel;
    }
    HANDLE VectorCreate();
    HANDLE VectorCreate(const int initialSize);
    void VectorDelete(const HANDLE _handle);
    bool VectorGet(const HANDLE _handle, const int index, void** ret);
    bool VectorSet(const HANDLE _handle, const int index, void* data);
    bool VectorSet(const HANDLE _handle, const int index, void* data, dealloc_t dealloc);
    bool VectorPushBack(const HANDLE _handle, void* data);
    bool VectorPushBack(const HANDLE _handle, void* data, dealloc_t dealloc);
    bool VectorPopBack(const HANDLE _handle);
    bool VectorInsert(const HANDLE _handle, const int position, void* data);
    bool VectorInsert(const HANDLE _handle, const int position, void* data, dealloc_t dealloc);
    bool VectorInsert(const HANDLE _handle, const int position, const int size, void* data);
    bool VectorInsert(const HANDLE _handle, const int position, const int size, void* data, dealloc_t dealloc);
    bool VectorErase(const HANDLE _handle, const int position);
    bool VectorErase(const HANDLE _handle, const int start, const int end);
    bool VectorSize(const HANDLE _handle, int* size);
    bool VectorEmpty(const HANDLE _handle, bool* b);
    bool VectorReserve(const HANDLE _handle, const int size);
    bool VectorResize(const HANDLE _handle, const int size);
    bool VectorFront(const HANDLE _handle, void** ret);
    bool VectorBack(const HANDLE _handle, void** ret);
};

#endif
