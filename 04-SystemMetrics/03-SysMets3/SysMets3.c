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
        TEXT("Get System Metrics 3"),
        WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
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
    static int  cxChar, cyChar, cxCaps, cxClient, cyClient, iMaxWidth;
    HDC         hdc;
    int         i, x, y, iVertPos, iHorzPos, iPaintBeg, iPaintEnd;
    PAINTSTRUCT ps;
    SCROLLINFO  si;
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

            //save width of three columns
            iMaxWidth = 40 * cxChar + 22 * cxCaps;
            return (0);
        
        case WM_SIZE:
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);

            //set veritical scroll bar range and page size
            si.cbSize = sizeof(si);
            si.fMask = SIF_RANGE | SIF_PAGE;
            si.nMin = 0;
            si.nMax = NUMLINES - 1;
            si.nPage = cyClient / cyChar;

            SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

            //set horizontal scroll bar range and page size
            si.cbSize = sizeof(si);
            si.fMask = SIF_RANGE | SIF_PAGE;
            si.nMin = 0;
            si.nMax = 2 + iMaxWidth / cxChar;
            si.nPage = cxClient / cxChar;
            SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);

            return (0);

        case WM_VSCROLL:
            //get all the vertical scroll bar information
            si.cbSize = sizeof(si);
            si.fMask = SIF_ALL;
            GetScrollInfo(hwnd, SB_VERT, &si);

            //save the position for comparison later on
            iVertPos = si.nPos;

            switch(LOWORD(wParam))
            {
                case SB_TOP:
                    si.nPos = si.nMin;
                    break;
                
                case SB_BOTTOM:
                    si.nPos = si.nMax;
                    break;
                
                case SB_LINEUP:
                    si.nPos -= 1;
                    break;
                
                case SB_LINEDOWN:
                    si.nPos += 1;
                    break;

                case SB_PAGEUP:
                    si.nPos -= si.nPage;
                    break;
                
                case SB_PAGEDOWN:
                    si.nPos += si.nPage;
                    break;
                
                case SB_THUMBTRACK:
                    si.nPos = si.nTrackPos;
                    break;
                
                default:
                    break;
            }

            //Set the positions and retrive it as due to
            //adjustments windows may change it

            si.fMask = SIF_POS;
            SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
            GetScrollInfo(hwnd, SB_VERT, &si);

            //if the positions has changed, scroll the window and update it

            if(si.nPos != iVertPos)
            {
                ScrollWindow(hwnd, 0, cyChar * (iVertPos - si.nPos), NULL, NULL);
                UpdateWindow(hwnd);
            }
            return (0);

        case WM_HSCROLL:
            //get all the horizontal scroll bar information
            si.cbSize = sizeof(si);
            si.fMask = SIF_ALL;

            //save the position for comparison later on

            GetScrollInfo(hwnd, SB_HORZ, &si);
            iHorzPos = si.nPos;

            switch(LOWORD(wParam))
            {
                case SB_LINELEFT:
                    si.nPos -= 1;
                    break;
                
                case SB_LINERIGHT:
                    si.nPos += 1;
                    break;
                
                case SB_PAGELEFT:
                    si.nPos -= si.nPage;
                    break;
                
                case SB_PAGERIGHT:
                    si.nPos += si.nPage;
                    break;
                
                case SB_THUMBPOSITION:
                    si.nPos = si.nTrackPos;
                    break;

                default:
                    break;
            }

            //set the position then retrive it because windows
            //may adjust the values 

            si.fMask = SIF_POS;
            SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
            GetScrollInfo(hwnd, SB_HORZ, &si);

            //if positions has changed scroll the window

            if(si.nPos != iHorzPos)
            {
                ScrollWindow(hwnd, cxChar * (iHorzPos - si.nPos), 0, NULL, NULL);
            }
            return (0);

        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);

            SetBkColor(hdc, RGB(0, 0, 0));
            SetTextColor(hdc, RGB(0, 255, 0));

            //get vertical scroll bar position

            si.cbSize = sizeof(si);
            si.fMask = SIF_POS;
            GetScrollInfo(hwnd, SB_VERT, &si);
            iVertPos = si.nPos;

            //get horizontal scroll bar position
            GetScrollInfo(hwnd, SB_HORZ, &si);
            iHorzPos = si.nPos;

            //find painting limits
            iPaintBeg = max(0, iVertPos + ps.rcPaint.top / cyChar);
            iPaintEnd = min(NUMLINES - 1, iVertPos + ps.rcPaint.bottom / cyChar);

            for(i = iPaintBeg; i <= iPaintEnd; i++)
            {
                x = cxChar * (1 - iHorzPos);
                y = cyChar * (i - iVertPos);

                TextOut(hdc, x, y, sysmetrics[i].szLabel, lstrlen(sysmetrics[i].szLabel));
                TextOut(hdc, x + 22 * cxCaps, y, sysmetrics[i].szDesc, lstrlen(sysmetrics[i].szDesc));

                SetTextAlign(hdc, TA_RIGHT | TA_TOP);
                TextOut(hdc, x + 22 * cxCaps + 40 * cxChar, y, szBuffer, wsprintf(szBuffer, TEXT("%5d"), GetSystemMetrics(sysmetrics[i].iIndex)));
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