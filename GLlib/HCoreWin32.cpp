
#include <windows.h>

#include "HCore.h"
#include "HCoreWin32.h"

static HWND g_hWnd = NULL;
static HINSTANCE g_hInstance = NULL;
static HANDLE g_ThreadHandle = NULL;
static unsigned long g_ThreadID = 0;

static int g_iWindowPosX = 100;
static int g_iWindowPosY = 100;
static int g_iWindowWidth = 512;
static int g_iWindowHeight = 512;

void HCoreSetWindowHandleWin32(HWND hWnd)
{
	g_hWnd = hWnd;
}

HWND HCoreGetWindowHandleWin32(void)
{
	return g_hWnd;
}

HINSTANCE HCoreGetWindowInstanceWin32(void)
{
	return g_hInstance;
}

void HCoreGetWindowSize(int &w, int &h)
{
	w = g_iWindowWidth;
	h = g_iWindowHeight;
}

void HCoreGetMouseState(int &x, int &y, int button[3])
{
	POINT p;

	GetCursorPos(&p);
	ScreenToClient(g_hWnd, &p);
	
	x = p.x;
	y = p.y;
	
	button[0] = GetKeyState(VK_LBUTTON) & 0x80 ? 1 : 0;
	button[1] = GetKeyState(VK_MBUTTON) & 0x80 ? 1 : 0;
	button[2] = GetKeyState(VK_RBUTTON) & 0x80 ? 1 : 0;
}

static LRESULT WINAPI WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}

		case WM_SIZE: 
		{
			int w = LOWORD(lParam);
			int h = HIWORD(lParam);
			g_iWindowWidth = w;
			g_iWindowHeight = h;
			if ( g_HCoreCallBack.OnSize && HCoreGetGraphicsDeviceType()!=HCore_UNKNOWN ) 
			{
				g_HCoreCallBack.OnSize(w, h);
			}
			break;
		}

		case WM_PAINT: 
		{
			PAINTSTRUCT ps;
			BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
			if ( g_HCoreCallBack.OnPaint )
			{
				g_HCoreCallBack.OnPaint();
			}
			break;
		}

		default: 
		{
			return DefWindowProc(hwnd, message, wParam, lParam);
			break;
		}
	}

	return 0;
} 

void HCoreCloseWindow(void)
{
	PostQuitMessage(0);
}


bool HCoreProcessMessage(void)
{
	MSG	msg;
	
	if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if ( msg.message==WM_QUIT )
		{
			return false;
		}
	}

	return true;
}

bool HCoreFullScreen(int width, int height)
{
	char *title = "fullscreen";

	DEVMODE dmScreenSettings;								
	memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	
	dmScreenSettings.dmSize=sizeof(dmScreenSettings);		
	dmScreenSettings.dmPelsWidth	= width;				
	dmScreenSettings.dmPelsHeight	= height;				
	dmScreenSettings.dmBitsPerPel	= 32;					
	dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

	if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
	{
		return false;
	}


	WNDCLASS	window_class;

	memset(&window_class, 0, sizeof(WNDCLASS));

	window_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = WndProc;
	window_class.hInstance = GetModuleHandle(NULL);
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	window_class.lpszClassName = title;
	if (RegisterClass(&window_class) == 0)
		return 0;
	
	DWORD dwExStyle=WS_EX_APPWINDOW;	
	DWORD dwStyle=WS_POPUP;				
	ShowCursor(FALSE);					

	RECT		WindowRect;				

	WindowRect.left=(long)0;			
	WindowRect.right=(long)width;		
	WindowRect.top=(long)0;				
	WindowRect.bottom=(long)height;		

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);	

	g_iWindowPosX = g_iWindowPosY = 0;
	g_iWindowWidth = width;
	g_iWindowHeight = height;

	HWND window_handle = CreateWindowEx(	
		dwExStyle,
		"fullscreen",
		title,
		dwStyle |
		WS_CLIPSIBLINGS |
		WS_CLIPCHILDREN,
		0, 0,
		WindowRect.right-WindowRect.left,
		WindowRect.bottom-WindowRect.top,
		NULL,
		NULL,
		window_class.hInstance,
		NULL
		);

	if (window_handle == NULL)
		return false;

	g_hWnd = window_handle;
	g_hInstance = window_class.hInstance;
	
	ShowWindow(window_handle, SW_SHOWNORMAL);
	SetActiveWindow(window_handle);
	SetForegroundWindow(window_handle);	
	SetFocus(window_handle);
	
	return true;
}


bool HCoreCreateWindow(int x, int y, int width, int height, const char *title)
{
	static bool registered = false;

	WNDCLASS	window_class;

	memset(&window_class, 0, sizeof(WNDCLASS));


	window_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = WndProc;
	window_class.hInstance = GetModuleHandle(NULL);
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	window_class.lpszClassName = title;
	if ( !registered )
	{
		if (RegisterClass(&window_class) == 0)
			return false;
		registered = true;
	}

	DWORD window_style;

	if ( g_HCoreCallBack.OnSize )
	{
		window_style = WS_OVERLAPPEDWINDOW;
	}
	else
	{
		window_style = WS_BORDER | WS_SYSMENU;
	}

	if ( x<0 || y<0 )
	{
		x = g_iWindowPosX;
		y = g_iWindowPosY;
	}

	if ( width<0 || height < 0 )
	{
		width = g_iWindowWidth;
		height = g_iWindowHeight;
	}

	RECT window_rect;
	SetRect(&window_rect, x, y, x+width, y+height);
	AdjustWindowRect(&window_rect, window_style, FALSE);
	
	g_iWindowPosX = x;
	g_iWindowPosY = y;
	g_iWindowWidth = width;
	g_iWindowHeight = height;

	HWND window_handle = CreateWindowEx(
		WS_EX_APPWINDOW,
		title,
		title,
		window_style,
		window_rect.left,	// x
		window_rect.top,	// y
		window_rect.right - window_rect.left,	// width
		window_rect.bottom - window_rect.top,	// height
		NULL,
		NULL,
		window_class.hInstance,
		NULL
		);

	if (window_handle == NULL)
		return false;

	g_hWnd = window_handle;
	g_hInstance = window_class.hInstance;

	ShowWindow(window_handle, SW_SHOWNORMAL);
	SetActiveWindow(window_handle);
	
	return true;
}
