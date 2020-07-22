//headers
#include <windows.h>


//macros
#define WIN_WIDTH  800
#define WIN_HEIGHT 600
#define NUMLINES ((int) (sizeof(devcaps) / sizeof(devcaps[0])))


//global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


//global variable declaration
struct
{
    int     iIndex;
    TCHAR  *szLabel;
    TCHAR  *szDesc;
}
devcaps[] = 
{
    HORZSIZE,       TEXT ("HORZSIZE"),      TEXT ("Width in millimeters:"),
    VERTSIZE,       TEXT ("VERTSIZE"),      TEXT ("Height in millimeters:"),
    HORZRES,        TEXT ("HORZRES"),       TEXT ("Width in pixels:"),
    VERTRES,        TEXT ("VERTRES"),       TEXT ("Height in raster lines:"),
    BITSPIXEL,      TEXT ("BITSPIXEL"),     TEXT ("Color bits per pixel:"),
    PLANES,         TEXT ("PLANES"),        TEXT ("Number of color planes:"),
    NUMBRUSHES,     TEXT ("NUMBRUSHES"),    TEXT ("Number of device brushes:"),
    NUMPENS,        TEXT ("NUMPENS"),       TEXT ("Number of device pens:"),
    NUMMARKERS,     TEXT ("NUMMARKERS"),    TEXT ("Number of device markers:"),
    NUMFONTS,       TEXT ("NUMFONTS"),      TEXT ("Number of device fonts:"),
    NUMCOLORS,      TEXT ("NUMCOLORS"),     TEXT ("Number of device colors:"),
    PDEVICESIZE,    TEXT ("PDEVICESIZE"),   TEXT ("Size of device structure:"),
    ASPECTX,        TEXT ("ASPECTX"),       TEXT ("Relative width of pixel:"),
    ASPECTY,        TEXT ("ASPECTY"),       TEXT ("Relative height of pixel:"),
    ASPECTXY,       TEXT ("ASPECTXY"),      TEXT ("Relative diagonal of pixel:"),
    LOGPIXELSX,     TEXT ("LOGPIXELSX"),    TEXT ("Horizontal dots per inch:"),
    LOGPIXELSY,     TEXT ("LOGPIXELSY"),    TEXT ("Vertical dots per inch:"),       
    SIZEPALETTE,    TEXT ("SIZEPALETTE"),   TEXT ("Number of palette entries:"),
    NUMRESERVED,    TEXT ("NUMRESERVED"),   TEXT ("Reserved palette entries:"),
    COLORRES,       TEXT ("COLORRES"),      TEXT ("Actual color resolution:")
};


//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpszCmdLine, int iCmdShow)
{
    //variable declaration
    WNDCLASS wndclass;
    HWND hwnd;
    MSG msg;
    TCHAR szAppName[] = TEXT("MyApp");
    int cxScreen, cyScreen;

    //code
    //initialization of wndclass
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
    RegisterClass(&wndclass);

    cxScreen = GetSystemMetrics(SM_CXSCREEN);
    cyScreen = GetSystemMetrics(SM_CYSCREEN);

    //create window
    hwnd = CreateWindow(szAppName,
        TEXT("My Application : DevCaps"),
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


//WndProc()
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    //variable declaration
    static int  cxChar, cxCaps, cyChar;
    TCHAR       szBuffer[10];
    HDC         hdc;
    int         i;
    PAINTSTRUCT ps;
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
            hdc = BeginPaint(hwnd, &ps);

            SetBkColor(hdc, RGB(0, 0, 0));
            SetTextColor(hdc, RGB(0, 0, 255));
            for(i = 0; i < NUMLINES; i++)
            {
                TextOut(hdc, 0, cyChar * i, devcaps[i].szLabel, lstrlen(devcaps[i].szLabel));
                TextOut(hdc, 14 * cxCaps, cyChar * i, devcaps[i].szDesc, lstrlen(devcaps[i].szDesc));
                SetTextAlign(hdc, TA_RIGHT | TA_TOP);
                TextOut(hdc, 14 * cxCaps + 35 * cxChar, cyChar * i, szBuffer, wsprintf(szBuffer, TEXT("%5d"), GetDeviceCaps(hdc, devcaps[i].iIndex)));
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