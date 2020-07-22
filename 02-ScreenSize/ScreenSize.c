//headers
#include <windows.h>
#include <tchar.h>
#include <stdio.h>


//global function declaration
int CDECL MessageBoxPrintf(TCHAR*, TCHAR*, ...);


//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    //variable declaration
    int cxScreen, cyScreen;

    //code
    cxScreen = GetSystemMetrics(SM_CXSCREEN);
    cyScreen = GetSystemMetrics(SM_CYSCREEN);

    MessageBoxPrintf(TEXT("ScreenSize"), TEXT("Screen Resolution is %i x %i (width x height) pixels."), cxScreen, cyScreen);

    return (0);
}

int CDECL MessageBoxPrintf(TCHAR *szCaption, TCHAR *szFormat, ...)
{
    //variable declaration
    TCHAR szBuffer[1024];
    va_list pArgList;

    //code
    va_start(pArgList, szFormat);

    _vsntprintf(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), szFormat, pArgList);

    va_end(pArgList);

    return (MessageBox(NULL, szBuffer, szCaption, 0));
}