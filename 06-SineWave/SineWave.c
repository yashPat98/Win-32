//headers
#include <windows.h>
#include <math.h>

//macros
#define WIN_WIDTH  800
#define WIN_HEIGHT 600
#define NUM        1000
#define TWOPI      (2 * 3.14159)


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
    wndclass.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszClassName  = szAppName;
    wndclass.lpszMenuName   = NULL;

    //register above class
    if(!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("This Program requires Windows NT!"), szAppName, MB_ICONERROR);
        return (0);
    }

    cxScreen = GetSystemMetrics(SM_CXSCREEN);
    cyScreen = GetSystemMetrics(SM_CYSCREEN);

    //create window
    hwnd = CreateWindow(szAppName,
        TEXT("My Application : Sine Wave"),
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
    static int  cxClient, cyClient;
    HDC         hdc;
    int         i;
    PAINTSTRUCT ps;
    POINT       apt[NUM];

    //code
    switch(iMsg)
    {
        case WM_SIZE:
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
            return (0);
    
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);

            MoveToEx(hdc, 0, cyClient / 2, NULL);
            LineTo(hdc, cxClient, cyClient / 2);

            for(i = 0; i < NUM; i++)
            {
                apt[i].x = i * cxClient / NUM;
                apt[i].y = (int)(cyClient / 2 * (1 - sin(TWOPI * i / NUM)));
            }

            Polyline(hdc, apt, NUM);
            return (0);

        case WM_DESTROY:
            PostQuitMessage(0);
            return (0);

        default:
            break;
    }

    return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}
