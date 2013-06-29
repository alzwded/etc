#ifndef mapModelHeader
#define mapModelHeader(TT_A, TT_N) \
class mapModel##TT_N { \
private: \
    map<HANDLE, map<const TT_A, element_t, less<const TT_A> > > maps; \
    HANDLE lastHandle; \
    int lastError; \
 \
    mapModel##TT_N(); \
public: \
    static mapModel##TT_N& getModel()  \
    {  \
        static mapModel##TT_N* theModel = new mapModel##TT_N(); \
        return *theModel; \
    } \
    HANDLE MapCreate(); \
    bool MapAccess(const HANDLE _handle, const TT_A key, void** ret); \
    bool MapHasKey(const HANDLE _handle, const TT_A key, bool* ret); \
    bool MapRemove(const HANDLE _handle, const TT_A key); \
    bool MapInsert(const HANDLE _handle, const TT_A key, const void* data); \
    bool MapInsert(const HANDLE _handle, const TT_A key, const void* data, void(*dealloc)(void**)); \
    void MapDelete(const HANDLE _handle); \
 \
    int getLastError() { int ret = lastError; return ret; } \
}; 
#endif
