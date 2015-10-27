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

#ifndef FU_WIN32_MSVC
# error "wrong platform"
#endif

#include <list>

// ==========================================================
// internal stuff
// ==========================================================

// after FU_ResizeFile is called, all Mmapped_t things returned
// need to be adjusted (i.e. deleted and recreated) so we return
// a pointer to the client code, and keep the real data here
// so that it can be adjusted
namespace {
    typedef std::list<Mmapped_t> mappedList_t;
}

static inline mappedList_t& GetMMappedList()
{
    static mappedList_t mappedList;
    return mappedList;
}

// translate GetLastError to something readable
static inline LPTSTR GetLastErrorString()
{
    // http://stackoverflow.com/questions/1387064/how-to-get-the-error-message-from-the-error-code-returned-by-getlasterror
    //Get the error message, if any.
    DWORD errorMessageID = ::GetLastError();
    if(errorMessageID == 0)
        return _T("");

    LPTSTR messageBuffer = nullptr;
    size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&messageBuffer, 0, NULL);

    return messageBuffer;

#if 0
    // I don't need that
    std::string message(messageBuffer, size);

    //Free the buffer.
    LocalFree(messageBuffer);

    return message;
#endif
}

// create a view of the mapped file
static inline void createView(Mmapped_t mapping)
{
    Offset_t offset = mapping->offset;
    Size_t length = mapping->length;
    FileHandle_t fh = mapping->fh;

    // fix offset
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    DWORD dwPageSize = si.dwPageSize;
    DWORD adjustment = offset % dwPageSize;
    offset = offset - adjustment;

    DWORD dwFileOffsetHigh = (offset >> 32) & 0xFFFFFFFFul;
    DWORD dwFileOffsetLow = offset & 0xFFFFFFFFul;
    SIZE_T dwNumberOfBytesToMap = length + adjustment;

    DWORD dwDesiredAccess = 0;
    switch(fh.mode) {
    case FU_READ:
        dwDesiredAccess = FILE_MAP_READ;
        break;
    case FU_WRITE:
        dwDesiredAccess = FILE_MAP_READ | FILE_MAP_WRITE;
        break;
    case FU_RW:
        dwDesiredAccess = FILE_MAP_READ | FILE_MAP_WRITE;
        break;
    }

    // call MapViewOfFile
    LPVOID ptr = MapViewOfFile(
            *fh.fh.hMapping,
            dwDesiredAccess,
            dwFileOffsetHigh,
            dwFileOffsetLow,
            dwNumberOfBytesToMap);

    if(ptr == NULL) {
        ErrorExit(GetLastErrorString());
    }

    mapping->base = ptr;
    mapping->ptr = Void_p_t((char*)ptr + adjustment);
}

// (re)create the file mapping
static inline void createFileMapping(FileHandle_t& fh)
{
    DWORD flProtect;

    // translate mode to the appropriate flags
    switch(fh.mode) {
    case FU_READ:
        flProtect = PAGE_READONLY;
        break;
    case FU_WRITE:
        flProtect = PAGE_READWRITE;
        break;
    case FU_RW:
        flProtect = PAGE_READWRITE;
        break;
    }

    // call CreateFileMapping
    SetLastError(ERROR_SUCCESS);

    HANDLE hMapping = CreateFileMapping(
            fh.fh.hFile,
            NULL,
            flProtect,
            0, // high
            0, // low; 0 means use everything
            NULL);

    if(GetLastError() == ERROR_ALREADY_EXISTS)
    {
        ErrorExit(GetLastErrorString());
    }

    if(hMapping == MMAPPED_NULL_HANDLE) {
        ErrorExit(GetLastErrorString());
    }

    *fh.fh.hMapping = hMapping;
}

// ==========================================================
// implementation of public API
// ==========================================================

FileHandle_t FU_OpenFile(StringType_t fname, FileMode_t mode)
{
    DWORD dwDesiredAccess = 0;
    DWORD dwSharedMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
    DWORD dwCreationDisposition = 0;
    DWORD dwFlagsAndAttributes = 0;
    DWORD flProtect;
    FileHandle_t ret;

    ret.mode = mode;

    // translate mode to the appropriate flags
    switch(mode) {
    case FU_READ:
        dwDesiredAccess = GENERIC_READ;
        dwCreationDisposition = OPEN_EXISTING;
        dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS;
        break;
    case FU_WRITE:
        dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
        dwCreationDisposition = CREATE_ALWAYS;
        dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS;
        break;
    case FU_RW:
        dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
        dwCreationDisposition = OPEN_ALWAYS;
        dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS;
        break;
    }

    // call CreateFile
    SetLastError(ERROR_SUCCESS);

    ret.fh.hFile = CreateFile(
            fname,
            dwDesiredAccess,
            dwSharedMode,
            NULL,
            dwCreationDisposition,
            dwFlagsAndAttributes,
            NULL);

    DWORD lastError = GetLastError();
    if(ret.fh.hFile == INVALID_HANDLE_VALUE
        || (lastError != ERROR_SUCCESS
            && lastError != ERROR_ALREADY_EXISTS))
    {
        ErrorExit(GetLastErrorString());
    }

    // we don't appreciate files of 0 length
    DWORD fileSizeLow = 0, fileSizeHigh = 0;
    fileSizeLow = GetFileSize(ret.fh.hFile, &fileSizeHigh);
    if(fileSizeLow == INVALID_FILE_SIZE) {
        ErrorExit(GetLastErrorString());
    }
    if(fileSizeLow == 0 && fileSizeHigh == 0) {
        if(mode == FU_READ) {
            CloseHandle(ret.fh.hFile);
            ErrorExit(_T("Specified file has 0 length"));
        } else {
            // truncate file to 64MB
            SetFilePointer(ret.fh.hFile, 64ul * 1024ul * 1024ul, NULL, FILE_BEGIN);
            SetEndOfFile(ret.fh.hFile);
            SetFilePointer(ret.fh.hFile, 0, NULL, FILE_BEGIN);
            uint32_t buf = 0;
            DWORD nbOfBytesWritten = 0;
            WriteFile(ret.fh.hFile, &buf, sizeof(uint32_t), &nbOfBytesWritten, NULL);
            SetFilePointer(ret.fh.hFile, 0, NULL, FILE_BEGIN);
        }
    }

    // instantiate the pointer to our mapping handle
    ret.fh.hMapping = new HANDLE;

    // create the file mapping
    createFileMapping(ret);

    return ret;
}

void FU_CloseFile(FileHandle_t fh)
{
    // 1. Close file mapping
    BOOL hr = CloseHandle(*fh.fh.hMapping);
    delete fh.fh.hMapping;
    if(!hr) {
        ErrorExit(GetLastErrorString());
    }

    // 2. Close file
    hr = CloseHandle(fh.fh.hFile);
    if(!hr) {
        ErrorExit(GetLastErrorString());
    }
}

Mmapped_t FU_MapViewOfFile(FileHandle_t fh, Size_t length, Offset_t offset)
{
    // add to list
    Mmapped_t ret = new MmappedPrivate_t;

    ret->offset = offset;
    ret->length = length;
    ret->fh = fh;

    // create view of file
    createView(ret);

    GetMMappedList().push_back(ret);

    return ret;
}

void FU_UnmapViewOfFile(Mmapped_t ptr)
{
    if(ptr == MMAPPED_NULL_HANDLE)
    {
        ErrorExit(_T("NULL ptr"));
    }

    BOOL hr = UnmapViewOfFile(ptr->base);
    if(!hr) {
        ErrorExit(GetLastErrorString());
    }
    GetMMappedList().erase(
            std::find(
                GetMMappedList().begin(),
                GetMMappedList().end(),
                ptr));
    delete ptr;
}

void FU_ResizeFile(FileHandle_t fh, Offset_t offset)
{
    if(offset == 0) {
        ErrorExit(_T("I refuse to truncate file to 0"));
    }

    // 1. close all mappings for the current file
    for(auto&& m : GetMMappedList()) {
        if(m->fh.fh.hFile != fh.fh.hFile) continue;
        if(m->base) UnmapViewOfFile(m->base);
    }
    CloseHandle(*fh.fh.hMapping);

    // 2. truncate file
    LARGE_INTEGER liOffset;
    liOffset.HighPart = (offset >> 32) & 0xFFFFFFFF;
    liOffset.LowPart = offset & 0xFFFFFFFF;
    DWORD hr = SetFilePointerEx(fh.fh.hFile, liOffset, NULL, FILE_BEGIN);
    if(hr == INVALID_SET_FILE_POINTER) {
        ErrorExit(GetLastErrorString());
    }
    hr = SetEndOfFile(fh.fh.hFile);
    if(hr == FALSE) {
        ErrorExit(GetLastErrorString());
    }

    // 3. recreate all mappings
    createFileMapping(fh);
    for(auto&& m : GetMMappedList()) {
        if(m->fh.fh.hFile != fh.fh.hFile) continue;
        if(m->offset + m->length <= offset) {
            createView(m);
        } else {
            m->base = NULL;
            m->ptr = NULL;
        }
    }
}
