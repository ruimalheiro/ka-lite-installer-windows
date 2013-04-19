#ifndef traynotification
#define traynotification

// Tray icon ID.
#define ID_TRAY_APP_ICON 5000

// Private windows message.
#define WM_TRAYICON ( WM_USER + 1 )

// Tray notify structure.
NOTIFYICONDATA notifyIconData;



/*************************************************************************************
*                                                                                    *
*	Functions declaration.                                                           *
*                                                                                    *
**************************************************************************************/
void InitNotifyIconData(HWND hwnd, HINSTANCE hINSTANCE);
void Minimize(HWND hwnd, char* title, char * message);
void Restore(HWND hwnd);
void sendTrayMessage(HWND hwnd, char* title, char * message);
void destroyTrayIcon();



/*************************************************************************************
*                                                                                    *
*	Inits the structure that will handle tray notification.                          *
*                                                                                    *
**************************************************************************************/
void InitNotifyIconData(HWND hwnd, HINSTANCE hINSTANCE)
{
	// Allocate memory for the structure.
	memset(&notifyIconData, 0, sizeof(NOTIFYICONDATA));
	notifyIconData.cbSize = sizeof(NOTIFYICONDATA);

	// Bind the NOTIFYICONDATA structure to our global hwnd ( handle to main window ).
	notifyIconData.hWnd = hwnd;

	// Set the NOTIFYICONDATA ID. HWND and uID form a unique identifier for each item in system tray.
	notifyIconData.uID = ID_TRAY_APP_ICON;

	// Set up flags.
	notifyIconData.uFlags = NIF_ICON    | // Guarantees that the hIcon member will be a valid icon.
		                    NIF_MESSAGE | // When someone clicks in the system tray icon, we want a WM_ type message to be sent to our WNDPROC
		                    NIF_TIP     | 
							NIF_INFO    | 
							NIF_SHOWTIP;  // Show tooltip.

	// This message must be handled in hwnd's window procedure.
	notifyIconData.uCallbackMessage = WM_TRAYICON;

	// Load the icon as a resource.
	notifyIconData.hIcon = LoadIcon(hINSTANCE, MAKEINTRESOURCE(IDI_ICON1));

	// Set the tooltip text.
	lstrcpy(notifyIconData.szTip, L"KA Lite");

	// Time to display the tooltip.
	notifyIconData.uTimeout = 100;

	// Type of tooltip (balloon).
	notifyIconData.dwInfoFlags = NIIF_INFO;

	// Copy text to the structure.
	lstrcpy(notifyIconData.szInfo, L"");
	lstrcpy(notifyIconData.szInfoTitle, L"");

	Shell_NotifyIcon(NIM_ADD, &notifyIconData);
}



/*************************************************************************************
*                                                                                    *
*	This function minimize to tray showing a specific message and title.             *
*                                                                                    *
**************************************************************************************/
void Minimize(HWND hwnd, char* title, char * message)
{
	// Add the icon to the system tray
	wchar_t  wtitle[100];
	swprintf(wtitle, 100, L"%hs", title);

	wchar_t wmessage[200];
	swprintf(wmessage, 200, L"%hs", message);

	lstrcpy(notifyIconData.szInfoTitle , wtitle);
	lstrcpy(notifyIconData.szInfo, wmessage);

	Shell_NotifyIcon(NIM_MODIFY, &notifyIconData);

	// and hide the main window
	ShowWindow(hwnd, SW_HIDE);
}



/*************************************************************************************
*                                                                                    *
*	Restore the window.                                                              *
*                                                                                    *
**************************************************************************************/
void Restore(HWND hwnd)
{
	// Show the main window
	ShowWindow(hwnd,SW_SHOW);
}



/*************************************************************************************
*                                                                                    *
*	Show any message on tray.                                                        *
*                                                                                    *
**************************************************************************************/
void sendTrayMessage(HWND hwnd, char* title, char * message)
{
	wchar_t  wtitle[100];
	swprintf(wtitle, 100, L"%hs", title);

	wchar_t wmessage[200];
	swprintf(wmessage,200,L"%hs", message);

	lstrcpy(notifyIconData.szInfoTitle , wtitle);
	lstrcpy(notifyIconData.szInfo, wmessage);

	Shell_NotifyIcon(NIM_MODIFY, &notifyIconData);			
}



/*************************************************************************************
*                                                                                    *
*	Destroy the tray icon.                                                           *
*                                                                                    *
**************************************************************************************/
void destroyTrayIcon()
{
	Shell_NotifyIcon(NIM_DELETE, &notifyIconData);
}

#endif