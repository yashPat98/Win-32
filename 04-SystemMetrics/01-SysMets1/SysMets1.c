//headers
#include <windows.h>
#include "SysMets.h"


//macros
#define WIN_WIDTH  800
#define WIN_HEIGHT 600


//global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    //variable declaration
    WNDCLASS wndclass;
    HWND hwnd;
    MSG msg;
    static TCHAR szAppName[] = TEXT("HelloWin");
    int cxScreen, cyScreen;

    //code
    //initialization of WNDCLASS
    wndclass.style          = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc    = WndProc;
    wndclass.cbClsExtra     = 0;
    wndclass.cbWndExtra     = 0;
    wndclass.hInstance      = hInstance;
    wndclass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.lpszClassName  = szAppName;
    wndclass.lpszMenuName   = NULL;

    //register above class
    if(!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("This Program requires Windows NT!"), szAppName, MB_ICONERROR);
        return (0);
    }

    cxScreen = (GetSystemMetrics(SM_CXSCREEN));
    cyScreen = (GetSystemMetrics(SM_CYSCREEN));

    //create window
    hwnd = CreateWindow(szAppName,
        TEXT("Get System Metrics 1"),
        WS_OVERLAPPEDWINDOW,
        (cxScreen - WIN_WIDTH) / 2,
        (cyScreen - WIN_HEIGHT) / 2,
        WIN_WIDTH,
        WIN_HEIGHT,
        NULL,
        NULL,
        hInstance,
        NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    //message loop
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return ((int)msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    //variable declaration
    static int  cxChar, cxCaps, cyChar;
    HDC         hdc;
    int         i;
    PAINTSTRUCT ps;
    TCHAR       szBuffer[10];
    TEXTMETRIC  tm;

    //code
    switch(iMsg)
    {   
        case WM_CREATE:
            hdc = GetDC(hwnd);

            GetTextMetrics(hdc, &tm);
            cxChar = tm.tmAveCharWidth;
            cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
            cyChar = tm.tmHeight + tm.tmExternalLeading;

            ReleaseDC(hwnd, hdc);
            return (0);

        case WM_PAINT:
            hdc = BeginPaint(hwnd ,&ps);

            SetBkColor(hdc, RGB(0, 0, 0));
            SetTextColor(hdc, RGB(0, 255, 0));
            for(i = 0; i < NUMLINES; i++)
            {
                TextOut(hdc, 0, cyChar * i, sysmetrics[i].szLabel, lstrlen(sysmetrics[i].szLabel));
                TextOut(hdc, 22 * cxCaps, cyChar * i, sysmetrics[i].szDesc, lstrlen(sysmetrics[i].szDesc));
                SetTextAlign(hdc, TA_RIGHT | TA_TOP);
                TextOut(hdc, 22 * cxCaps + 40 * cxChar, cyChar * i, szBuffer, wsprintf(szBuffer, TEXT("%5d"), GetSystemMetrics(sysmetrics[i].iIndex)));
                SetTextAlign(hdc, TA_LEFT | TA_TOP);
            }
            EndPaint(hwnd, &ps);
            return (0);

        case WM_DESTROY:
            PostQuitMessage(0);
            return (0);

        default:
            break;
    }

    return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}
