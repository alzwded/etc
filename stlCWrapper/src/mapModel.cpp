#ifndef mapModel
#define mapModel(TT_A, TT_N) \
mapModel##TT_N::mapModel##TT_N() \
{ \
    lastHandle = 1; \
    lastError = 0; \
} \
 \
HANDLE mapModel##TT_N::MapCreate() \
{ \
    if(lastHandle < MAX_HANDLE) { \
        maps.insert(pair<HANDLE, map<const TT_A, element_t, less<const TT_A> > >(lastHandle, map<const TT_A, element_t, less<const TT_A> >())); \
        lastError = 0; \
        return lastHandle++; \
    } else { \
        lastHandle = 1; \
        while(maps.find(lastHandle) != maps.end() && lastHandle < MAX_HANDLE) { \
            lastHandle ++; \
        } \
        if(lastHandle < MAX_HANDLE) { \
            maps.insert(pair<HANDLE, map<const TT_A, element_t, less<const TT_A> > >(lastHandle, map<const TT_A, element_t, less<const TT_A> >())); \
            lastError = 0; \
            return lastHandle++; \
        } else { \
            /* TODO implement error codes */ \
            return NULL_HANDLE; \
        } \
    } \
} \
 \
bool mapModel##TT_N::MapInsert(const HANDLE _handle, const TT_A key, const void* data) \
{ \
    if(maps.find(_handle) == maps.end()) { \
        return false; \
    } \
    maps[_handle][key] = element_t(const_cast<void*>(data)); \
    lastError = 0; \
    return true; \
} \
 \
bool mapModel##TT_N::MapInsert(const HANDLE _handle, const TT_A key, const void* data, void(*dealloc)(void**)) \
{ \
    if(maps.find(_handle) == maps.end()) { \
        return false; \
    } \
    maps[_handle][key] = element_t(const_cast<void*>(data), dealloc); \
    lastError = 0; \
    return true; \
} \
 \
bool mapModel##TT_N::MapRemove(const HANDLE _handle, const TT_A key) \
{ \
    if(maps.find(_handle) == maps.end()) { \
        return false; \
    } \
    lastError = 0; \
    map<const TT_A, element_t>::iterator toRemove = maps[_handle].find(key); \
    maps[_handle].erase(toRemove); \
    return true; \
} \
 \
bool mapModel##TT_N::MapHasKey(const HANDLE _handle, const TT_A key, bool* ret) \
{ \
    if(maps.find(_handle) == maps.end()) { \
        return false; \
    } \
    *ret = (maps[_handle].find(key) != maps[_handle].end()); \
    lastError = 0; \
    return true; \
} \
 \
bool mapModel##TT_N::MapAccess(const HANDLE _handle, const TT_A key, void** ret) \
{ \
    bool has = false;; \
    if(!MapHasKey(_handle, key, &has) || !has) { \
        return false; \
    } \
 \
    *ret = (maps[_handle][key].data); \
    lastError = 0; \
    return true; \
} \
 \
void mapModel##TT_N::MapDelete(const HANDLE _handle) \
{ \
    map<HANDLE, map<const TT_A, element_t> >::iterator i; \
    if((i = maps.find(_handle)) != maps.end()) { \
        maps.erase(i); \
    } \
} 
#endif
