#ifndef __D3DAPPLICATION_H__
#define __D3DAPPLICATION_H__

#include "Context.h"
#include <Windows.h>

namespace OGE
{
	class WindowsApplication : public Context
	{
	public:

		WindowsApplication();
		~WindowsApplication();

		bool initialize(HINSTANCE hInstance, uint32 cmdShow, WNDPROC = WindowsApplication::WndProc);
		void start();
		void shutdown();

	protected:

		virtual void performApplicationLogic() {}

	private:
		void destroy();
		ATOM registerWindowClass(HINSTANCE hInstance, WNDPROC WndProc);

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		HINSTANCE hInstance;
		HWND hWnd;
		WCHAR* windowClassName;
	};

	inline WindowsApplication::WindowsApplication()
	{
		windowClassName = L"d3d_app";
	}

	inline WindowsApplication::~WindowsApplication() {}
}

#endif

