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
#include <FileUtils.h>

#ifndef FU_LINUX_GCC
# error "wrong platform"
#endif

#include <sys/queue.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <errno.h>
#include <fcntl.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// ==========================================================
// internal stuff
// ==========================================================

// after FU_ResizeFile is called, all Mmapped_t things returned
// need to be adjusted (i.e. deleted and recreated) so we return
// a pointer to the client code, and keep the real data here
// so that it can be adjusted
struct FU_MmappedEntry_t;
static STAILQ_HEAD(FU_MmappedList_t, FU_MmappedEntry_t) FU_mmappedList
        = STAILQ_HEAD_INITIALIZER(FU_mmappedList);
struct FU_MmappedEntry_t {
    MmappedPrivate_t value;
    STAILQ_ENTRY(FU_MmappedEntry_t) entries;
};
typedef struct FU_MmappedEntry_t FU_MmappedEntry_t;
static _Bool initialized = false;

static void uninitializeList(void)
{
     struct FU_MmappedEntry_t* n1 = STAILQ_FIRST(&FU_mmappedList), *n2;
     while (n1 != NULL) {
         n2 = STAILQ_NEXT(n1, entries);
         free(n1);
         n1 = n2;
     }
     STAILQ_INIT(&FU_mmappedList);
}

static void initializeList(void)
{
    if(!initialized) {
        STAILQ_INIT(&FU_mmappedList);
        atexit(&uninitializeList);
        initialized = true;
    }
}

static void createView(FU_MmappedEntry_t* entry)
{
    off_t offset = entry->value.offset;
    size_t length = entry->value.length;
    FileHandle_t fh = entry->value.fh;

    int prot = 0; // file mode for mmap
    int flags = 0; // flags for mmap
    void* ptr = NULL; // return value of mmap
    Offset_t alignedOffset = offset; // offset passed to mmap
    const long sz = sysconf(_SC_PAGE_SIZE);  // system page size
    off_t alignedMask = sz - 1; // mask to test whether an
                                   // address if page aligned
    off_t addressAdjustment = 0; // adjustment offset to make an address
                                 // page aligned

    // application constraint: memory should be 32bit aligned
    if(offset & 0x1F) {
        free(entry);
        ErrorExit("Unaligned offset requested");
    }

    // mmap requirement: memory is aligned
    alignedOffset = offset & (~alignedMask);
    addressAdjustment = offset & alignedMask;

    // convert mode to prot and flags
    switch(fh.mode) {
    case FU_READ:
        prot = PROT_READ;
        flags = MAP_PRIVATE | MAP_NORESERVE;
        break;
    case FU_WRITE:
        prot = PROT_WRITE;
        flags = MAP_SHARED;
        break;
    case FU_RW:
        prot = PROT_READ | PROT_WRITE;
        flags = MAP_SHARED;
        break;
    }

    // call mmap
    ptr = mmap(
            NULL,
            length + addressAdjustment, // adjust length since offset may
                                        // start at a lower address
                                        // FIXME adjust length to page boundary
            prot,
            flags,
            fh.fh,
            alignedOffset);

    if(ptr == MAP_FAILED) {
        free(entry);
        ErrorExit(strerror(errno));
    }


    // store the base pointer (which will get passed to munmap)...
    entry->value.base = ptr;
    // ... and the actual pointer we'll be using
    entry->value.ptr = (entry->value.base + addressAdjustment);
}

// ==========================================================
// implementation of public API
// ==========================================================

FileHandle_t FU_OpenFile(StringType_t fname, FileMode_t mode)
{
    initializeList();

    mode_t pmode = 0;
    int flags = 0;
    FileHandle_t ret;

    ret.mode = mode;

    // convert mode to UNIX open mode
    switch(mode) {
    case FU_READ:
        flags = O_RDONLY;
        break;
    case FU_WRITE:
        flags = O_WRONLY | O_CREAT | O_TRUNC;
        pmode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
        break;
    case FU_RW:
        flags = O_RDWR | O_CREAT;
        pmode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
        break;
    }

    // open the file
    ret.fh = open(fname, flags, pmode);

    if(ret.fh == -1) {
        ErrorExit(strerror(errno));
    }

    // check file size
    struct stat st;
    fstat(ret.fh, &st);
    if(mode != FU_READ) {
        uint32_t buf = 0;
        if(st.st_size == 0) {
            // truncate file to 64MB
            ftruncate(ret.fh, 64ul * 1024ul * 1024ul);
        }
        lseek(ret.fh, 0, SEEK_SET);
        write(ret.fh, &buf, sizeof(uint32_t));
        lseek(ret.fh, 0, SEEK_SET);
    } else {
        close(ret.fh);
        ErrorExit("File has size 0");
    }

    return ret;
}

void FU_CloseFile(FileHandle_t fh)
{
    initializeList();
    int hr = close(fh.fh);
    if(hr == -1) {
        ErrorExit(strerror(errno));
    }
}

Mmapped_t FU_MapViewOfFile(FileHandle_t fh, Size_t length, Offset_t offset)
{
    initializeList();

    struct FU_MmappedEntry_t* entry = (struct FU_MmappedEntry_t*)malloc(sizeof(struct FU_MmappedEntry_t));

    entry->value.fh = fh;
    entry->value.length = length;
    entry->value.offset = offset;
    createView(entry);

    // add entry to the list
    STAILQ_INSERT_TAIL(&FU_mmappedList, entry, entries);

    // return the pointer to the MmappedPriv_t entry;
    // we will juggle some pointers around later to retrieve the entry
    return &entry->value;
}

void FU_UnmapViewOfFile(Mmapped_t mapping)
{
    if(mapping == MMAPPED_NULL_HANDLE)
    {
        ErrorExit("NULL ptr");
    }

    // some pointer ju[ng]gling to get our FU_mmappedList pointer
    // normaly, offsetof would not be necessary, but let's write
    // good portable code
    void* adjustedPointer = ((void*)mapping - offsetof(struct FU_MmappedEntry_t, value));
    struct FU_MmappedEntry_t* entry = (struct FU_MmappedEntry_t*)adjustedPointer;

    int hr = 0;
    if(mapping->base) {
        // do nothing if the mapping was effed during a file resize operation
        hr = munmap(mapping->base, mapping->length);
    }

    // remove the entry
    STAILQ_REMOVE(&FU_mmappedList, entry, FU_MmappedEntry_t, entries);
    free(entry);

    // check for munmap errors
    if(hr == -1) {
        ErrorExit(strerror(errno));
    }
}

void FU_ResizeFile(FileHandle_t fh, Offset_t newSize)
{
    initializeList();

    if(newSize == 0) {
        ErrorExit("I refuse to truncate file to 0");
    }
    // 1. close all mappings for the current file
    struct FU_MmappedEntry_t* np;
    STAILQ_FOREACH(np, &FU_mmappedList, entries) {
        if(np->value.fh.fh != fh.fh) continue;
        // do nothing if mapping was effed during a resize operation
        int hr = 0;
        if(np->value.base) hr = munmap(np->value.base, np->value.length);
        if(hr != 0) {
            ErrorExit(strerror(errno));
        }
    }

    // 2. truncate file
    ftruncate(fh.fh, newSize);

    // 3. recreate all mappings
    STAILQ_FOREACH(np, &FU_mmappedList, entries) {
        if(np->value.fh.fh != fh.fh) continue;
        if(np->value.offset + np->value.length <= newSize) {
            createView(np);
        } else {
            np->value.base = NULL;
            np->value.ptr = NULL;
        }
    }
}
