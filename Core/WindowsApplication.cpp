#include "WindowsApplication.h"

namespace OGE
{
	bool WindowsApplication::initialize(HINSTANCE hInstance, uint32 cmdShow, WNDPROC WndProc)
	{
		this->hInstance = hInstance;

		ATOM uid = registerWindowClass(hInstance, WndProc);

		if(uid == 0)
		{
			error_msg("cannot register window class");
		}

		hWnd = CreateWindow(windowClassName, windowClassName,
			WS_OVERLAPPEDWINDOW, 
			0,
			0,
			800,
			600,
			0,
			0,
			hInstance, 
			NULL);

		if (!hWnd)
		{
			error_msg("window is not created");
			return FALSE;
		}
		
		ShowWindow(hWnd, cmdShow);
		UpdateWindow(hWnd);

		startManagers(hInstance, hWnd);

		return TRUE;
	}

	void WindowsApplication::start()
	{
		MSG msg;

		for(;;)
		{
			if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			{
				GetMessage(&msg, NULL, 0, 0);
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				if(msg.message == WM_QUIT)
				{
					shutdown();
					break;
				}
				
			}
			else
			{
				performApplicationLogic();
			}
		}
	}

	void WindowsApplication::shutdown()
	{
		destroy();
	}

	void WindowsApplication::destroy()
	{
		shutdownManagers();
	}

	ATOM WindowsApplication::registerWindowClass(HINSTANCE hInstance, WNDPROC WndProc)
	{
		WNDCLASSEX wc;
		
		ZeroMemory(&wc, sizeof(wc));
		
		wc.cbSize = sizeof(wc); 
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0; 
		wc.cbWndExtra = 0; 
		wc.hInstance = hInstance; 
		wc.lpszMenuName = NULL; 
		wc.lpszClassName = windowClassName; 

		return RegisterClassEx(&wc);
	}

	LRESULT CALLBACK WindowsApplication::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}
}	