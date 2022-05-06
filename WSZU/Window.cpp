#include "Window.h"
#include "WndProc.h"

namespace WI
{
	WNDCLASSEX wcex = {};
	HINSTANCE hInstance = GetModuleHandle(nullptr);

	VOID InicializedWcex()
	{
		memset(&wcex, 0, sizeof(WNDCLASS));
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW - 4);
	}

	VOID create_window(LPCWSTR Name_Class_Wnd, LPCWSTR Name_Window,
		DWORD H, DWORD W)
	{
		wcex.lpszClassName = Name_Class_Wnd;
		if (!RegisterClassEx(&wcex))
			return;
		HWND hwnd = CreateWindowEx(
			0, Name_Class_Wnd, Name_Window, WS_MINIMIZEBOX | WS_SYSMENU | WS_CLIPCHILDREN,
			CW_USEDEFAULT, CW_USEDEFAULT, H, W, HWND_DESKTOP, NULL, hInstance, NULL
		);
		if (!hwnd)
			return;
		//SetTimer(hwnd, 1, 10, NULL);
		SetTimer(hwnd, 0, 0, NULL);
		ShowWindow(hwnd, SW_SHOWNORMAL);
		MSG msg = {};
		while (true)
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				if (msg.message == WM_QUIT)
					break;
			}
		return;
	}
}