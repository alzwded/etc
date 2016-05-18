// Tile2D.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Tile2D.h"
#include <deque>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// timer for updates
UINT_PTR pTime = NULL;
// tile map
int M[30][40];
// internal state
int seed = 0;
// graph curve
std::deque<int> L;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	 WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	 About(HWND, UINT, WPARAM, LPARAM);

// generic update callback
VOID CALLBACK Update(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    // do some internal update
    seed = (seed + 1) % 1200;

    int x = seed / 40;
    int y = seed % 40;
    int c = (seed % 4) + 1;

    // draw the tile map
    memset(M, 0, sizeof(int) * 40 * 30);
    M[x][y] = c;

    // add a sample to the line buffer
    L.pop_front();
    L.push_back((int)(50 * sin(seed / 20.0)));

    // signal the window needs to be redrawn
    RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);

    // reset timer
    SetTimer(hWnd, idEvent, 20, &Update);
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TILE2D, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TILE2D));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TILE2D));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TILE2D);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   // instantiate our line to be 400 values long
   L = decltype(L)(400);
   // initialize update timer
   pTime = SetTimer(hWnd, NULL, 20, &Update);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void Paint(HDC& hDC, PAINTSTRUCT ps)
{
    // create drawing area for tile map
    auto memDC = CreateCompatibleDC(hDC);
    HBITMAP hBM = CreateCompatibleBitmap(hDC, 400, 300);

    // switch to that
    SelectObject(memDC, hBM);

    // fill with gray background
    RECT r;
    SetRect(&r, 0, 0, 400, 300);
    FillRect(memDC, &r, (HBRUSH)GetStockObject(GRAY_BRUSH));

    // load a bitmap; this should be global to avoid I/O
    auto bmp = (HBITMAP)LoadImage(hInst, _T("c:\\bitmap.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    // read the bitmap header because we need width and height
    BITMAP bm;
    GetObject(bmp, sizeof(BITMAP), &bm);

    // create a DC for the bitmap
    auto bmpDC = CreateCompatibleDC(memDC);
    // switch our bitmap in
    SelectObject(bmpDC, bmp);
    // copy bitmap to memDC (over the gray background)
    BitBlt(memDC, 50, 50, bm.bmWidth, bm.bmHeight, bmpDC, 0, 0, SRCCOPY);

    // switch to memDC
    SelectObject(memDC, hBM);

    // cleanup
    DeleteDC(bmpDC);
    DeleteObject(bmp);

    // create some coloured brushes
    HBRUSH brushes[5] = {
        CreateSolidBrush(RGB(128, 128, 128)),
        CreateSolidBrush(RGB(255, 0, 0)),
        CreateSolidBrush(RGB(0, 255, 0)),
        CreateSolidBrush(RGB(120, 100, 0)),
        CreateSolidBrush(RGB(0, 0, 255))
    };
    // go over the tile map
    for(size_t i = 0; i < 30; ++i) {
        for(size_t j = 0; j < 40; ++j) {
            // setup our tile size
            RECT r;
            SetRect(&r, j * 10, i * 10, (j + 1) * 10, (i + 1) * 10);
            // draw a coloured pixel
            if(M[i][j]) FillRect(memDC, &r, brushes[M[i][j]]);
        }
    }
    // cleanup brushes
    for(size_t i = 0; i < 5; ++i) DeleteObject(brushes[i]);

    // copy buffer onto screen
    BitBlt(hDC, 0, 0, 400, 300, memDC, 0, 0, SRCCOPY);

    // create DC for the graph
    auto memDC2 = CreateCompatibleDC(hDC);
    HBITMAP hBM2 = CreateCompatibleBitmap(hDC, 400, 300);

    // switch to that
    SelectObject(memDC2, hBM2);

    // clear background
    SetRect(&r, 0, 0, 400, 300);
    FillRect(memDC2, &r, (HBRUSH)GetStockObject(WHITE_BRUSH));

    // iterate over line L
    int x = 0;
    for(auto i: L)
    {
        int y = i + 50;
        // draw a green pixel
        SetPixel(memDC2, x, y, RGB(100, 255, 100));
        ++x;
    }

    // copy graph onto screen below the tile map
    BitBlt(hDC, 0, 300, 400, 100, memDC2, 0, 0, SRCCOPY);

    // cleanup (I hope I got everything; leaks in a timed loop are aweful)
    DeleteObject(hBM);
    DeleteDC(memDC);
    DeleteObject(hBM2);
    DeleteDC(memDC2);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

        Paint(hdc, ps);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
