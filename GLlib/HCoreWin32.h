#ifdef WIN32

#include <windows.h>

#ifndef _HCoreILWIN32_
#define _HCoreILWIN32_

HWND HCoreGetWindowHandleWin32(void);
void HCoreSetWindowHandleWin32(HWND hWnd);
void HCoreCloseWindow(void);
HINSTANCE HCoreGetWindowInstanceWin32(void);
void HCoreGetWindowSize(int &w, int &h);

#endif // _HCoreILWIN32_

#endif // WIN32
