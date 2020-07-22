//headers
#include <windows.h>
#include "SysMets.h"


//macros
#define WIN_WIDTH  800
#define WIN_HEIGHT 600


//global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    //variable declaration
    WNDCLASS wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR szAppName[] = TEXT("MyApp");
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

    //registering above class
    RegisterClass(&wndclass);

    cxScreen = GetSystemMetrics(SM_CXSCREEN);
    cyScreen = GetSystemMetrics(SM_CYSCREEN);

    //create window
    hwnd = CreateWindow(szAppName,
        TEXT("Get System Metrics 2"),
        WS_OVERLAPPEDWINDOW | WS_VSCROLL,
        (cxScreen - WIN_WIDTH) / 2,
        (cyScreen = WIN_HEIGHT) / 2,
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


//WndProc()
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    //variable declaration
    static int  cxChar, cyChar, cxCaps, cyClient, iVscrollPos;
    HDC         hdc;
    int         i, y;
    PAINTSTRUCT ps;
    TCHAR       szBuffer[10];
    TEXTMETRIC  tm;

    switch(iMsg)
    {
        case WM_CREATE:
            hdc = GetDC(hwnd);

            GetTextMetrics(hdc, &tm);
            cxChar = tm.tmAveCharWidth;
            cxCaps = ((tm.tmPitchAndFamily & 1) ? 3 : 2) * cxChar / 2;
            cyChar = tm.tmHeight + tm.tmExternalLeading;

            ReleaseDC(hwnd, hdc);

            SetScrollRange(hwnd, SB_VERT, 0, NUMLINES - 1, FALSE);
            SetScrollPos(hwnd, SB_VERT, iVscrollPos, TRUE);
            return (0);
        
        case WM_SIZE:
            cyClient = HIWORD(lParam);
            return (0);
        
        case WM_VSCROLL:
            switch(LOWORD(wParam))
            {
                case SB_LINEUP:
                    iVscrollPos -= 1;
                    break;
                
                case SB_LINEDOWN:
                    iVscrollPos += 1;
                    break;
                
                case SB_PAGEUP:
                    iVscrollPos -= cyClient / cyChar;
                    break;
                
                case SB_PAGEDOWN:
                    iVscrollPos += cyClient / cyChar;
                    break;

                case SB_THUMBPOSITION:
                    iVscrollPos = HIWORD(wParam);
                    break;
                
                default:
                    break;
            }

            iVscrollPos = max(0, min(iVscrollPos, NUMLINES - 1));

            if(iVscrollPos != GetScrollPos(hwnd, SB_VERT))
            {
                SetScrollPos(hwnd, SB_VERT, iVscrollPos, TRUE);
                InvalidateRect(hwnd, NULL, TRUE);
                UpdateWindow(hwnd);
            }
            return (0);

        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);

            SetBkColor(hdc, RGB(0, 0, 0));
            SetTextColor(hdc, RGB(0, 255, 0));
            for(i = 0; i < NUMLINES; i++)
            {
                y = cyChar * (i - iVscrollPos);

                TextOut(hdc, 0, y, sysmetrics[i].szLabel, lstrlen(sysmetrics[i].szLabel));
                TextOut(hdc, 22 * cxCaps, y, sysmetrics[i].szDesc, lstrlen(sysmetrics[i].szDesc));

                SetTextAlign(hdc, TA_RIGHT | TA_TOP);
                TextOut(hdc, 22 * cxCaps + 40 * cxChar, y, szBuffer, wsprintf(szBuffer, TEXT("%5d"), GetSystemMetrics(sysmetrics[i].iIndex)));
                SetTextAlign(hdc, TA_LEFT | TA_TOP);
            }
            EndPaint(hwnd, &ps);
            return (0);

        case WM_DESTROY:
            PostQuitMessage(0);
            return (0);

    }

    return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}