#ifndef fle_win32_framework
#define fle_win32_framework

#include <Windows.h>
#include <stdio.h>
#include <map>

#include "resource.h"

#define WM_TRAYICON ( WM_USER + 1 )

UINT CURRENT_VALID_ID = WM_TRAYICON + 1;

class fle_BaseWindow
{
	private:
		HINSTANCE hInstance;
		HWND hwnd;

		std::map<int, fle_BaseWindow*> children_map;
		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	public:
		fle_BaseWindow(HINSTANCE, int, int, TCHAR*, TCHAR*);
		void show(void);
		void test(void);
};

fle_BaseWindow::fle_BaseWindow(HINSTANCE hInstance, int WIDTH, int HEIGHT, TCHAR * CLASS_NAME, TCHAR * TITLE)
{
	hInstance = hInstance;
	WNDCLASSEX wc = {0};

	// Registering the window class.
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = &fle_BaseWindow::WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) COLOR_APPWORKSPACE;
	wc.lpszClassName = CLASS_NAME;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if(!RegisterClassEx(&wc)){
		MessageBox(NULL, L"Failed to register the window.", L"Error", MB_ICONEXCLAMATION | MB_OK);
	}

	// Creating the window.
	DWORD windowStyle = WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU;
	this -> hwnd = CreateWindowEx(NULL, CLASS_NAME, TITLE, windowStyle, CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT, NULL,  NULL, hInstance, NULL);	

	if(hwnd == NULL){
		MessageBox(NULL, L"Failed to create the window.", L"Error", MB_ICONEXCLAMATION | MB_OK);
	}
}

void fle_BaseWindow::show()
{
	MSG Msg;

	ShowWindow(this->hwnd, SW_SHOW);

	while(GetMessage(&Msg, NULL, 0 , 0) > 0){
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
}

LRESULT CALLBACK fle_BaseWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
		{
			case WM_PAINT:
				{
			
				}
				break;

			case WM_CREATE:
				{
			
				}
				break;

			case WM_COMMAND:
				{
					switch(LOWORD(wParam))
						{
							//case ID_MINIMIZE_BUTTON:
							//case ID_OPEN_IN_BROWSER:
							//case ID_OPTIONS_RUNUSERLOGIN:
							//case ID_OPTIONS_RUNSTARTUP:
							//case ID_OPTIONS_AUTOMINIMIZE:
							//case ID_OPTIONS_AUTOSTART:
							//case ID_HELP_ABOUT:
							//case ID_FILE_EXIT:
							//case ID_STUFF_GO:
						}
				}
				break;

			case WM_TRAYICON:
				{
					switch(wParam)
						{
							//case ID_TRAY_APP_ICON:
						}		  
				}
				break;

			case WM_NCHITTEST:
				{
				}
				break;

			case WM_CLOSE:
				{
					PostQuitMessage(0);
				}
				break;

			case WM_DESTROY:
				{
				}
				break;

			default:
				{
					return DefWindowProc(hwnd, msg, wParam, lParam);
				}
		}

	return 0;
};

void fle_BaseWindow::test()
{
	static const TCHAR * str = TEXT("WELCOME FLE FRAMEWORK\n");
    OutputDebugString(str);
};





class fle_Window : public fle_BaseWindow
{
	public:
		fle_Window(HINSTANCE, int, int, TCHAR*, TCHAR*);
		void show(void);
};

fle_Window::fle_Window(HINSTANCE hInstance, int WIDTH, int HEIGHT, TCHAR * CLASS_NAME, TCHAR * TITLE) : fle_BaseWindow(hInstance, WIDTH, HEIGHT, CLASS_NAME, TITLE)
{

}

void fle_Window::show()
{
	fle_BaseWindow::show();
	
	//if(!IsWindowVisible(this->hwnd))
	//{
		//destroyTrayIcon();
	//}
	fle_BaseWindow::test();
}

#endif