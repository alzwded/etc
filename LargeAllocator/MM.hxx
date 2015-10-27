/*
Copyright (C) 2015 Vlad Mesco

This file is part of VoxelVolumizer.

VoxelVolumizer is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

VoxelVolumizer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with VoxelVolumizer.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef MM_HXX
#define MM_HXX

#define NOMINMAX
#include "FileUtils.h"

#include <cstdint>
#include <cinttypes>
#include <cstring>

#include <algorithm>
#include <exception>
#include <list>
#include <stdexcept>

#ifdef _MSC_VER
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#endif

#define MM_PageSize (UINT64_C(64 * 1024 * 1024))
#define MM_PageMask (UINT64_C(MM_PageSize - 1))
#define MM_BaseMask (~MM_PageMask)

struct MM
{
    typedef uint64_t Handle;

    MM(StringType_t file, int numCachePages = 8)
        : cache_()
          , currentTail_(0ul)
          , numPages_(numCachePages)
    {
        fh_ = FU_OpenFile(file, FU_RW);
        if(numPages_ <= 0) {
            throw std::invalid_argument("Invalid number of cache pages specified");
        }
    }

    ~MM()
    {
        // unmap all cached pages
        for(auto&& i : cache_) {
            FU_UnmapViewOfFile(i.second);
        }

        // close file
        FU_CloseFile(fh_);
    }

    Handle New(size_t size)
    {
        if(size > MM_PageSize) {
            throw std::invalid_argument("Cannot allocate more than 64MB (one page) at a time");
        }
        auto base = currentTail_ & MM_BaseMask;
        auto pp = currentTail_;
        if(pp + size - base > MM_PageSize) {
            base += MM_PageSize;
            FU_ResizeFile(fh_, base + MM_PageSize);

            auto newMap = FU_MapViewOfFile(fh_, MM_PageSize, base);
            Push(base, newMap);

            pp = base;
        }
        auto ptr = LoadPage(pp);
        memset((char*)ptr + pp - base, 0, size);
        currentTail_ = pp + size;
        if((currentTail_ & MM_BaseMask) - base > 0) { // corner case where someone's alloc'ing exact MM_PageSize
            FU_ResizeFile(fh_, (currentTail_ & MM_BaseMask) + MM_PageSize);
        }
        return pp;
    }

    template<typename T>
    T* Deref(Handle h)
    {
        auto mem = LoadPage(h);
        auto base = h & MM_BaseMask;
        auto off = h - base;
        return reinterpret_cast<T*>(((char*)mem) + off);
    }

    template<typename T, typename... ARGS>
    Handle Emplace(ARGS&&... args)
    {
        typedef std::remove_reference<T> Tnoref;
        Handle ret = New(sizeof(Tnoref));
        Tnoref* ptr = Deref<Tnoref>(ret);
        std::allocator<Tnoref>::construct(ptr, args...);
        return ret;
    }
    
    // TODO implement generic Free
    void Free(Handle h)
    {
        /*
            while implementing free allocation/deallocation malloc style
            would be a cool excercise, I really don't need it for this project

            this allocator will only be used for allocating REALLY large chunks of memory
        */
        throw std::runtime_error("Generic MM::Free not implemented");
    }

    // Specific Free: truncate memory from h upward
    void TruncateAt(Handle h)
    {
        auto base = h & MM_BaseMask;
        // purge cache entries referring to such a ludicrous address
        std::remove_if(cache_.begin(), cache_.end(),
            [base](decltype(cache_)::const_reference e) -> bool {
                return e.first > base;
            });
        // update current thing
        currentTail_ = h;
        // truncate file
        FU_ResizeFile(fh_, base + MM_PageSize + MM_PageSize * (base == h));
    }

    // Snip the memory in the [first, last) range
    // Invalidates all handles > first
    // The caller should adjust all pointers > last with
    //    h -= last - first;
    void TruncateRange(Handle first, Handle last)
    {
        auto p1 = first;
        auto p2 = last;
        auto totalLen = (currentTail_ & MM_BaseMask) + MM_PageSize;
        auto len = totalLen - last; 
        auto copied = UINT64_C(0);
        while(copied < len) {
            auto base1 = p1 & MM_BaseMask;
            auto base2 = p2 & MM_BaseMask;
            
            auto ptr1 = Deref<void*>(p1);
            auto ptr2 = Deref<void*>(p2);

            auto d1 = std::min(MM_PageSize - (p1 - base1), len - copied);
            auto d2 = std::min(MM_PageSize - (p2 - base2), len - copied);
            auto d = std::min(d1, d2);
            memmove(ptr1, ptr2, d);
            p1 += d;
            p2 += d;
            copied += d;
        }

        TruncateAt(first + len);
    }

    template<typename T>
    void Destroy(Handle h)
    {
        typedef std::remove_reference<T> Tnoref;
        Tnoref* ptr = Deref<Tnoref>(h);
        ptr->~Tnoref();
        memset(ptr, 0, sizeof(Tnoref));
    }

private:
    void Push(Handle base, Mmapped_t newMap)
    {
        cache_.push_front(std::make_pair(base, newMap));
        if(cache_.size() > numPages_) {
            FU_UnmapViewOfFile(cache_.back().second);
            cache_.pop_back();
        }
    }

    void* LoadPage(Handle h)
    {
        auto base = h & MM_BaseMask;
        auto&& found = std::find_if(cache_.begin(), cache_.end(),
                [base](std::pair<Handle, Mmapped_t> page) -> bool {
                    return page.first == base;
                });
        if(found != cache_.end()) return FU_GetPtr(found->second); // cache hit

        // cache miss; reload page
        Mmapped_t newMap = FU_MapViewOfFile(fh_, MM_PageSize, base);
        Push(base, newMap);
        return FU_GetPtr(newMap);
    }

private:
    FileHandle_t fh_;
    std::list<std::pair<Handle, Mmapped_t>> cache_;
    int numPages_;
    Handle currentTail_;
};

#endif
