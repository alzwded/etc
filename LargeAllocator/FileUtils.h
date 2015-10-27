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
#ifndef FILE_UTILS_H
#define FILE_UTILS_H

/*
    Utilities for working with file mappings in a cross-platform way.

    Utilities:
        FileHandle_t FU_OpenFile(StringType_t fname, FileMode_t mode);
            open a file
        void FU_CloseFile(FileHandle_t);
            close a file; make sure to unmap all views of that file
        Mmapped_t FU_MapViewOfFile(FileHandle_t, Size_t length, Offset_t offset);
            map a view of a file; open the file with FU_OpenFile
            offset should be aligned to a 32bit boundary
        void FU_UnmapViewOfFile(Mmapped_t);
            unmap a view of a file
        void FU_ResizeFile(FileHandle_t, Offset_t newSize);
            truncate a file to a certain size
            open the file with FU_OpenFile
        uint32_t FU_GetPtr(Mmapped_t) // macro
            get the pointer of a Mmapped_t handle
        #define MMAPPED_NULL_HANDLE
            A null mmapped handle
        FileMode_t
            FU_READ, FU_WRITE, FU_RW
        FileHandle_t
            handle to a file opened with FU_OpenFile
        Mmapped_t
            handle to a view of a mapped file
            created with FU_MapViewOfFile
            you can access the memory with FU_GetPtr
*/

#include <stdint.h>
#include <inttypes.h>

// ==========================================================
// Windows / MSVC specific defines
// ==========================================================
#ifdef _MSC_VER
# define FU_WIN32_MSVC
# define WIN32_LEAN_AND_MEAN
# define VC_EXTRALEAN
# ifndef UNICODE
#  define UNICODE
# endif
# ifndef _UNICODE
#  define _UNICODE
# endif
# include <Tchar.h>
# include <Windows.h>
typedef struct {
    HANDLE hFile;
    HANDLE* hMapping;
} FileHandleImpl_t;
typedef LPVOID Void_p_t;
typedef SIZE_T Size_t;
typedef int64_t Offset_t; // QWORD
typedef LPTSTR StringType_t;
#define FU_S(X) (_T(X))
// ==========================================================
// GCC / Linux specific defines
// ==========================================================
#elif defined(__GNUC__) //&& defined(__linux)
# define FU_LINUX_GCC
# include <unistd.h>
# if !defined(_POSIX_MAPPED_FILES) || _POSIX_MAPPED_FILES < 1
#  error "Unsupported non-posix system"
# endif
# include <sys/mman.h>
typedef int FileHandleImpl_t;
typedef void* Void_p_t;
typedef size_t Size_t;
typedef off_t Offset_t;
typedef const char* StringType_t;
#define FU_S(X) (X)
#else
// ==========================================================
// Other
// ==========================================================
# error "unsupported compiler"
#endif

// ==========================================================
// Platform independent stuff
// ==========================================================

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    FU_READ = 0,
    FU_WRITE,
    FU_RW
} FileMode_t;

typedef struct {
    FileHandleImpl_t fh;
    FileMode_t mode;
} FileHandle_t;

typedef struct {
    Void_p_t base;
    Void_p_t ptr;
    FileHandle_t fh;
    Size_t length;
    Offset_t offset;
} MmappedPrivate_t, *Mmapped_t;

// Get the pointer to the memory mapped region
#define FU_GetPtr(X) (X->ptr)
// A null mmapped handle
#define MMAPPED_NULL_HANDLE NULL

extern void ErrorExit(StringType_t message);

// Open a file which you can call MapViewOfFile on
extern FileHandle_t FU_OpenFile(StringType_t fname, FileMode_t mode);
// Close a file handle opened with FU_OpenFile
extern void FU_CloseFile(FileHandle_t);

// Map a view of a file
extern Mmapped_t FU_MapViewOfFile(FileHandle_t, Size_t length, Offset_t offset);
// Unmap a view of a file
extern void FU_UnmapViewOfFile(Mmapped_t);

// Truncate a file opened with FU_OpenFile
extern void FU_ResizeFile(FileHandle_t, Offset_t newSize);

#ifdef __cplusplus
}
#endif

#endif
