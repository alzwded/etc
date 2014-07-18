#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <Windows.h>
#include <Psapi.h>
#include <cstdlib>
#include <memory>
#include <iostream>
#include <strsafe.h>

void ErrorPrint(LPSTR lpszFunction) 
{ 
    // Retrieve the system error message for the last-error code

    LPSTR lpMsgBuf = NULL;
    LPSTR lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        lpMsgBuf,
        0, NULL );

    // Display the error message and exit the process

    lpDisplayBuf = (LPSTR)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlenA((LPCSTR)lpMsgBuf) + lstrlenA((LPCSTR)lpszFunction) + 40) * sizeof(CHAR)); 
    StringCchPrintfA((STRSAFE_LPSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        "%s failed with error %d: %s", 
        lpszFunction, dw, lpMsgBuf); 
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 
    std::cerr << lpDisplayBuf << std::endl;

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}

int main(int argc, char* argv[])
{
#define NUM (1024)
#define SIZE (NUM * sizeof(DWORD))
    DWORD* lpidProcess = (DWORD*)calloc(1024, sizeof(DWORD));
    DWORD lpcbNeeded(0);
    std::unique_ptr<DWORD> mem1(lpidProcess);
    BOOL hr = EnumProcesses(lpidProcess, SIZE, &lpcbNeeded);
    if(!hr) {
        std::cerr << "enum processes failed" << std::endl;
        return 255;
    }
    if(lpcbNeeded == SIZE) {
        std::cerr << "too many processes" << std::endl;
        return 1;
    }
    DWORD num = lpcbNeeded / sizeof(DWORD);

    for(size_t i = 0; i < num; ++i) {
        HANDLE hwnd = OpenProcess( PROCESS_QUERY_INFORMATION |
                                PROCESS_VM_READ | PROCESS_SET_QUOTA ,
                                FALSE, lpidProcess[i] );
        if(hwnd == NULL) continue; // probably no rights to open the process, d/c

        std::cout << "killing working set of " << hwnd << std::endl;

        hr = EmptyWorkingSet(hwnd);
        if(hr == 0) {
            CloseHandle(hwnd);
            std::cerr << "empty working set failed" << std::endl;
            ErrorPrint("empty working set");
            continue;
        }
        CloseHandle(hwnd);
    }

    return 0;
}