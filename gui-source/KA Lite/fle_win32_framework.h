#ifndef fle_win32_framework
#define fle_win32_framework

#include <Windows.h>
#include <stdio.h>
#include <map>

//#include "resource.h"

#define ID_TRAY_APP_ICON 5000
#define WM_TRAYICON ( WM_USER + 1 )

UINT CURRENT_VALID_ID = WM_TRAYICON + 1;

class fle_BaseWindow
{
	private:
		HINSTANCE * p_hInstance;
		HWND hwnd;

		std::map<int, fle_BaseWindow*> children_map;
		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	public:
		fle_BaseWindow(HINSTANCE*, int, int, TCHAR*, TCHAR*);
		void show(void);
		void test(void);
		HWND& getWindowReference(void);
		HINSTANCE* getInstanceReference(void);
};

fle_BaseWindow::fle_BaseWindow(HINSTANCE * hInstance, int WIDTH, int HEIGHT, TCHAR * CLASS_NAME, TCHAR * TITLE)
{
	p_hInstance = hInstance;
	WNDCLASSEX wc = {0};

	// Registering the window class.
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = &fle_BaseWindow::WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = *p_hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) COLOR_APPWORKSPACE;
	wc.lpszClassName = CLASS_NAME;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	//wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	//wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hIcon = NULL;
	wc.hIconSm = NULL;

	if(!RegisterClassEx(&wc)){
		MessageBox(NULL, L"Failed to register the window.", L"Error", MB_ICONEXCLAMATION | MB_OK);
	}

	// Creating the window.
	DWORD windowStyle = WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU;
	this -> hwnd = CreateWindowEx(NULL, CLASS_NAME, TITLE, windowStyle, CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT, NULL,  NULL, *p_hInstance, NULL);	

	if(hwnd == NULL){
		MessageBox(NULL, L"Failed to create the window.", L"Error", MB_ICONEXCLAMATION | MB_OK);
	}
}

HWND& fle_BaseWindow::getWindowReference()
{
	return this->hwnd;
}

HINSTANCE * fle_BaseWindow::getInstanceReference()
{
	return this->p_hInstance;
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





class fle_TrayWindow : public fle_BaseWindow
{
	private:
		NOTIFYICONDATA *notifyIconData;
		HINSTANCE * p_hInstance;
	public:
		fle_TrayWindow(HINSTANCE*);
		NOTIFYICONDATA* getNotifyIconDataStructure(void);
		HINSTANCE* getInstanceReference(void);
		void setTrayIcon(UINT);
		void show(void);
};

fle_TrayWindow::fle_TrayWindow(HINSTANCE * hInstance) : fle_BaseWindow(hInstance, 0, 0, L"DEFAULT", L"DEFAULT")
{
	p_hInstance = hInstance;

	// Allocate memory for the structure.
	//memset(notifyIconData, 0, sizeof(NOTIFYICONDATA));
	notifyIconData = (NOTIFYICONDATA*)malloc(sizeof(NOTIFYICONDATA));
	notifyIconData->cbSize = sizeof(NOTIFYICONDATA);

	// Bind the NOTIFYICONDATA structure to our global hwnd ( handle to main window ).
	notifyIconData->hWnd = fle_BaseWindow::getWindowReference();

	// Set the NOTIFYICONDATA ID. HWND and uID form a unique identifier for each item in system tray.
	notifyIconData->uID = ID_TRAY_APP_ICON;

	// Set up flags.
	notifyIconData->uFlags = NIF_ICON    | // Guarantees that the hIcon member will be a valid icon.
		                     NIF_MESSAGE | // When someone clicks in the system tray icon, we want a WM_ type message to be sent to our WNDPROC
		                     NIF_TIP     | 
							 NIF_INFO    | 
							 NIF_SHOWTIP;  // Show tooltip.

	// This message must be handled in hwnd's window procedure.
	notifyIconData->uCallbackMessage = WM_TRAYICON;

	// Set the tooltip text.
	lstrcpy(notifyIconData->szTip, L"KA Lite");

	// Time to display the tooltip.
	notifyIconData->uTimeout = 100;

	// Type of tooltip (balloon).
	notifyIconData->dwInfoFlags = NIIF_INFO;

	// Copy text to the structure.
	lstrcpy(notifyIconData->szInfo, L"");
	lstrcpy(notifyIconData->szInfoTitle, L"");
}

NOTIFYICONDATA* fle_TrayWindow::getNotifyIconDataStructure()
{
	return notifyIconData;
}

HINSTANCE* fle_TrayWindow::getInstanceReference()
{
	return p_hInstance;
}

void fle_TrayWindow::setTrayIcon(UINT icon_id)
{
	// Load the icon as a resource.
	fle_TrayWindow::getNotifyIconDataStructure()->hIcon = LoadIcon(*getInstanceReference(), MAKEINTRESOURCE(icon_id));
}

void fle_TrayWindow::show()
{
	Shell_NotifyIcon(NIM_ADD, notifyIconData);

	MSG Msg;

	while(GetMessage(&Msg, NULL, 0 , 0) > 0){
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	
	fle_BaseWindow::test();
}







class fle_Window : public fle_BaseWindow
{
	private:
		HINSTANCE * p_hInstance;
	public:
		fle_Window(HINSTANCE*, int, int, TCHAR*, TCHAR*);
		void show(void);
};

fle_Window::fle_Window(HINSTANCE * hInstance, int WIDTH, int HEIGHT, TCHAR * CLASS_NAME, TCHAR * TITLE) : fle_BaseWindow(hInstance, WIDTH, HEIGHT, CLASS_NAME, TITLE)
{
	p_hInstance = hInstance;
}

void fle_Window::show()
{
	MSG Msg;

	ShowWindow(fle_BaseWindow::getWindowReference(), SW_SHOW);

	while(GetMessage(&Msg, NULL, 0 , 0) > 0){
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	
	fle_BaseWindow::test();
}

#endif