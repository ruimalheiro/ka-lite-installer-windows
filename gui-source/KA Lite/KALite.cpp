#include <Windows.h>
#include "resource.h"
#include <stdio.h>

// Window Menu ID's
#define ID_FILE_EXIT 9001
#define ID_STUFF_GO 9002
#define ID_HELP_ABOUT 9003
#define ID_OPTIONS_RUNSTARTUP 9004
#define ID_OPTIONS_AUTOMINIMIZE 9005
#define ID_OPTIONS_AUTOSTART 9006

// Main Buttons ID's.
#define ID_START_BUTTON 1001
#define ID_STOP_BUTTON 1002
#define ID_MINIMIZE_BUTTON 1003
#define ID_OPEN_IN_BROWSER 1004
#define ID_START_STOP_BUTTON 1005

// Tray icon ID.
#define ID_TRAY_APP_ICON 5000

// Tray Menu options ID.
#define ID_TRAY_EXIT_CONTEXT_MENU_ITEM  3000
#define ID_TRAY_START_CONTEXT_MENU_ITEM 3001
#define ID_TRAY_STOP_CONTEXT_MENU_ITEM 3002
#define ID_TRAY_BROWSER_CONTEXT_MENU_ITEM 3003
#define ID_TRAY_RESTORE_CONTEXT_MENU_ITEM 3004
#define ID_TRAY_START_STOP_CONTEXT_MENU_ITEM 3005

// Private windows message.
#define WM_TRAYICON ( WM_USER + 1 )

// UINT that will hold a new defined message.
UINT WM_TASKBARCREATED = 0;

// Handle to main window.
HWND hwnd;

// Handle to window Menu.
HMENU hMenu;
HMENU hMainMenu;
HMENU hFileSubMenu;
HMENU hOptionsSubMenu;
HMENU hAboutSubMenu;

// Handle to main buttons.
HWND hstart;
HWND hstop;

// Device context ( used to paint ).
HDC hDC;

// Main module handle.
HINSTANCE hINSTANCE;

// Tray icon.
HBITMAP bitmap;

// Tray notify structure.
NOTIFYICONDATA g_notifyIconData;

// Structures to specify how will cmd and both python process run.
SHELLEXECUTEINFO cmdCommandsInfo;
SHELLEXECUTEINFO pythonShellExecuteInfo_1;
SHELLEXECUTEINFO pythonShellExecuteInfo_2;
SHELLEXECUTEINFO winshortcutInfo;

// Process ID.
DWORD cmdProcess;
DWORD processID1;
DWORD processID2;
DWORD winshortcutID;

// Process handle.
HANDLE cmdProcessHandle;
HANDLE processHandle1;
HANDLE processHandle2;
HANDLE winshortcutHandle;

// Component messages.
UINT startupCheckState;
UINT autoMinimizeCheckState;
UINT autoStartServer;

bool SERVERISRUNNING;
bool CHANGEDSTATE;
bool runServerOnCreationMessage;

char ReadBuffer[4];
TCHAR versionFinal[MAX_PATH] = {};
DWORD winshortcutReturn; 

// Function that creates the gui config file.
int setConfigFile(char * value)
{
	HANDLE hFile;
	char * DataBuffer = value;
	DWORD bytesToWrite = (DWORD) strlen(DataBuffer);
	DWORD bytesWritten = 0;
	BOOL errorFlag = FALSE;

	hFile = CreateFile(L"CONFIG.dat",GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, L"Failed to create config file", L"Error", MB_OK | MB_ICONERROR);
		CloseHandle(hFile);
		return 1;
	} else { 

		errorFlag = WriteFile(hFile, DataBuffer, bytesToWrite, &bytesWritten, NULL);

		if(FALSE == errorFlag)
		{
			MessageBox(NULL, L"Failed to write to config file", L"Error", MB_OK | MB_ICONERROR);
			CloseHandle(hFile);
			return 1;
		}

		CloseHandle(hFile);
		return 0;
	}
	CloseHandle(hFile);
	return 1;
}

// Function to check and read config file.
int readConfigFile()
{
	HANDLE hFile;
	DWORD bytesRead = 0;

	hFile = CreateFile(L"CONFIG.dat", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		// Fails to open the file but continues and create one.
		CloseHandle(hFile);
		return 1;
	}

	if(ReadFile(hFile, ReadBuffer, 3, &bytesRead, NULL) == FALSE)
	{
		MessageBox(NULL, L"Failed to read the config file", L"Error", MB_OK | MB_ICONERROR);
		CloseHandle(hFile);
		return 1;
	}

	if( bytesRead > 0 && bytesRead <= 3)
	{
		ReadBuffer[3] = '\0';
		if(ReadBuffer[0] == '0')
		{
			//Success
			CloseHandle(hFile);
			return 0;
		}
		CloseHandle(hFile);
		return 1;
	}
	else if( bytesRead == 0)
	{
		MessageBox(NULL, L"No data read from file", L"Error", MB_OK | MB_ICONERROR);
		CloseHandle(hFile);
		return 1;
	}
	else
	{
		MessageBox(NULL, L"Unexpected value", L"Error", MB_OK | MB_ICONERROR);
		CloseHandle(hFile);
		return 1;
	}
	CloseHandle(hFile);
	return 1;
}

int getVersionFromFile()
{
	HANDLE hFile;
	DWORD bytesRead = 0;
	char versionBuffer[30];

	hFile = CreateFile(L"ka-lite\\kalite\\version.py", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		// Fails to open the file but continues and create one.
		CloseHandle(hFile);
		return 1;
	}

	if(ReadFile(hFile, versionBuffer, 29, &bytesRead, NULL) == FALSE)
	{
		MessageBox(NULL, L"Failed to read the config file", L"Error", MB_OK | MB_ICONERROR);
		CloseHandle(hFile);
		return 1;
	}

	if( bytesRead > 0 && bytesRead <= 29)
	{
		versionBuffer[29] = '\0';
		int i = 0;
		for(i=0; i<29; i++)
		{
			if(versionBuffer[i] == '\"')
			{
				break;
			}
		}
		i++;
		int j = 0;
		for(j=0;j<29; j++)
		{
			if(versionBuffer[i] == '\"')
			{
				break;
			}
			versionFinal[j] = versionBuffer[i];
			i++;
		}
		versionFinal[j] = '\0';

		CloseHandle(hFile);
		return 0;
	}
	else if( bytesRead == 0)
	{
		MessageBox(NULL, L"No data read from file", L"Error", MB_OK | MB_ICONERROR);
		CloseHandle(hFile);
		return 1;
	}
	else
	{
		MessageBox(NULL, L"Unexpected value", L"Error", MB_OK | MB_ICONERROR);
		CloseHandle(hFile);
		return 1;
	}
	CloseHandle(hFile);
	return 1;
}

void refreshConfigBuffer()
{
	if ( ReadBuffer[0] == '0' )
	{
		startupCheckState = SW_SHOWNA;
	}
	if( ReadBuffer[1] == '0' )
	{
		autoMinimizeCheckState = SW_SHOWNA;
	}		
	if( ReadBuffer[2] == '0' )
	{
		autoStartServer = SW_SHOWNA;
	}
}

void prepareShellInfoStructures()
{
	cmdCommandsInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	cmdCommandsInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	cmdCommandsInfo.hwnd = NULL;
	cmdCommandsInfo.lpVerb = L"open";
	cmdCommandsInfo.lpFile = L"cmd.exe" ;
	cmdCommandsInfo.lpParameters = L"move static\\videos\\* ..\\content > nul 2> nul";
	cmdCommandsInfo.lpDirectory = L"ka-lite\\kalite\\";
	cmdCommandsInfo.nShow = SW_HIDE;
	cmdCommandsInfo.hInstApp = NULL;

	pythonShellExecuteInfo_1.cbSize = sizeof(SHELLEXECUTEINFO);
	pythonShellExecuteInfo_1.fMask = SEE_MASK_NOCLOSEPROCESS;
	pythonShellExecuteInfo_1.hwnd = NULL;
	pythonShellExecuteInfo_1.lpVerb = L"open";
	pythonShellExecuteInfo_1.lpFile = L"python.exe" ;
	pythonShellExecuteInfo_1.lpParameters = L"cronserver.py";
	pythonShellExecuteInfo_1.lpDirectory = L"ka-lite\\kalite\\";
	pythonShellExecuteInfo_1.nShow = SW_HIDE;
	pythonShellExecuteInfo_1.hInstApp = NULL;

	pythonShellExecuteInfo_2.cbSize = sizeof(SHELLEXECUTEINFO);
	pythonShellExecuteInfo_2.fMask = SEE_MASK_NOCLOSEPROCESS;
	pythonShellExecuteInfo_2.hwnd = NULL;
	pythonShellExecuteInfo_2.lpVerb = L"open";
	pythonShellExecuteInfo_2.lpFile = L"python.exe" ;
	pythonShellExecuteInfo_2.lpParameters = L"manage.py runcherrypyserver host=0.0.0.0 port=8008 threads=50";
	pythonShellExecuteInfo_2.lpDirectory = L"ka-lite\\kalite\\";
	pythonShellExecuteInfo_2.nShow = SW_HIDE;
	pythonShellExecuteInfo_2.hInstApp = NULL;
}

void setStartupShortcut(int v)
{
	winshortcutInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	winshortcutInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	winshortcutInfo.hwnd = NULL;
	winshortcutInfo.lpVerb = L"open";
	winshortcutInfo.lpFile = L"winshortcut.vbs" ;
	if(v == 0){
		winshortcutInfo.lpParameters = L"0";
	} else if (v == 1)
	{
		winshortcutInfo.lpParameters = L"1";
	}
	winshortcutInfo.lpDirectory = NULL;
	winshortcutInfo.nShow = SW_HIDE;
	winshortcutInfo.hInstApp = NULL;

	if(!ShellExecuteEx(&winshortcutInfo))
	{
		MessageBox(NULL, L"Failed to create startup shortcut!", L"Error", MB_OK | MB_ICONERROR);
	}
}

// Function to start the server.
void startServerCommand()
{
	if(ShellExecuteEx(&cmdCommandsInfo))
	{
		cmdProcess = GetProcessId(cmdCommandsInfo.hProcess);
		if(ShellExecuteEx(&pythonShellExecuteInfo_1))
		{
			processID1 = GetProcessId(pythonShellExecuteInfo_1.hProcess);
			if(ShellExecuteEx(&pythonShellExecuteInfo_2))
			{
				processID2 = GetProcessId(pythonShellExecuteInfo_2.hProcess);
				SERVERISRUNNING = TRUE;
				if(!runServerOnCreationMessage)
				{
					MessageBox(hwnd,L"The server should now be accessible locally at: http://127.0.0.1:8008/ or you can press \"Open KA Lite button\"", L"Server started", MB_OK | MB_ICONINFORMATION);
				}				
			}
			else
			{
				MessageBox(hwnd,L"Could not start the server! Step3", L"Starting error", MB_OK | MB_ICONERROR);
			}
		}
		else
		{
			MessageBox(hwnd,L"Could not start the server! Step2", L"Starting error", MB_OK | MB_ICONERROR);
		}		
	}
	else
	{
		MessageBox(hwnd,L"Could not start the server! Step1", L"Starting error", MB_OK | MB_ICONERROR);		
	}			
}

// Function to stop the server.
void stopServerCommand()
{
	processHandle1 = OpenProcess(PROCESS_TERMINATE, false,  processID1);
	processHandle2 = OpenProcess(PROCESS_TERMINATE, false, processID2);
	cmdProcessHandle = OpenProcess(PROCESS_TERMINATE, false,  cmdProcess);		
	TerminateProcess(processHandle1,1);
	TerminateProcess(processHandle2,1);
	TerminateProcess(cmdProcessHandle,1);		
	CloseHandle(processHandle1);
	CloseHandle(processHandle2);
	CloseHandle(cmdProcessHandle);
	SERVERISRUNNING = FALSE;
}

// Function to check the state of menu's.
void CheckMenus()
{
	if(SERVERISRUNNING)
	{
		EnableMenuItem(hMenu, ID_TRAY_START_CONTEXT_MENU_ITEM, MF_DISABLED | MF_GRAYED);
		EnableMenuItem(hMenu, ID_TRAY_STOP_CONTEXT_MENU_ITEM, MF_ENABLED);
		EnableMenuItem(hMenu, ID_TRAY_EXIT_CONTEXT_MENU_ITEM, MF_ENABLED);
		EnableMenuItem(hMenu, ID_TRAY_BROWSER_CONTEXT_MENU_ITEM, MF_ENABLED);
		EnableWindow(GetDlgItem(hwnd,ID_START_BUTTON), FALSE);
		EnableWindow(GetDlgItem(hwnd,ID_STOP_BUTTON), TRUE);
		EnableWindow(GetDlgItem(hwnd,ID_MINIMIZE_BUTTON), FALSE);
		EnableWindow(GetDlgItem(hwnd,ID_OPEN_IN_BROWSER), TRUE);
		EnableWindow(GetDlgItem(hwnd,ID_FILE_EXIT), FALSE);
		hMenu = CreatePopupMenu();
		AppendMenu(hMenu, MF_STRING, ID_TRAY_START_STOP_CONTEXT_MENU_ITEM, L"Stop server");											
		AppendMenu(hMenu, MF_STRING, ID_TRAY_BROWSER_CONTEXT_MENU_ITEM, L"Open KA Lite");
		AppendMenu(hMenu, MF_STRING, ID_TRAY_RESTORE_CONTEXT_MENU_ITEM, L"Restore window");
		AppendMenu(hMenu, MF_STRING, ID_TRAY_EXIT_CONTEXT_MENU_ITEM, L"Exit KA Lite");
		if(CHANGEDSTATE && IsWindowVisible(hwnd))
		{
			SetDlgItemTextW(hwnd, ID_START_STOP_BUTTON, L"Stop server");
			CHANGEDSTATE = false;
		}
	}
	else
	{
		EnableMenuItem(hMenu, ID_TRAY_START_CONTEXT_MENU_ITEM, MF_ENABLED);
		EnableMenuItem(hMenu, ID_TRAY_STOP_CONTEXT_MENU_ITEM, MF_DISABLED | MF_GRAYED);
		EnableMenuItem(hMenu, ID_TRAY_EXIT_CONTEXT_MENU_ITEM, MF_ENABLED);
		EnableMenuItem(hMenu, ID_TRAY_BROWSER_CONTEXT_MENU_ITEM, MF_DISABLED | MF_GRAYED);
		EnableWindow(GetDlgItem(hwnd,ID_START_BUTTON), TRUE);
		EnableWindow(GetDlgItem(hwnd,ID_STOP_BUTTON), FALSE);
		EnableWindow(GetDlgItem(hwnd,ID_MINIMIZE_BUTTON), TRUE);
		EnableWindow(GetDlgItem(hwnd,ID_OPEN_IN_BROWSER), FALSE);
		EnableWindow(GetDlgItem(hwnd,ID_FILE_EXIT), TRUE);
		hMenu = CreatePopupMenu();
		AppendMenu(hMenu, MF_STRING, ID_TRAY_START_STOP_CONTEXT_MENU_ITEM, L"Start server");											
		AppendMenu(hMenu, MF_STRING, ID_TRAY_BROWSER_CONTEXT_MENU_ITEM, L"Open KA Lite");
		AppendMenu(hMenu, MF_STRING, ID_TRAY_RESTORE_CONTEXT_MENU_ITEM, L"Restore window");
		AppendMenu(hMenu, MF_STRING, ID_TRAY_EXIT_CONTEXT_MENU_ITEM, L"Exit KA Lite");
		if(CHANGEDSTATE && IsWindowVisible(hwnd))
		{
			SetDlgItemTextW(hwnd, ID_START_STOP_BUTTON, L"Start server");
			CHANGEDSTATE = false;
		}
	}
}

// Functions to minimize and to restore from Tray.

void Minimize()
{
	// Add the icon to the system tray
	Shell_NotifyIcon(NIM_ADD, &g_notifyIconData);

	// and hide the main window
	ShowWindow(hwnd,SW_HIDE);
}

void Restore()
{
	// Remove the icon from the system tray
	Shell_NotifyIcon(NIM_DELETE, &g_notifyIconData);

	// and show the main window
	ShowWindow(hwnd,SW_SHOW);
}


// Initialize the NOTIFYICONDATA structure
void InitNotifyIconData()
{
	// Allocate memory for the structure.
	memset(&g_notifyIconData, 0, sizeof(NOTIFYICONDATA));
	g_notifyIconData.cbSize = sizeof(NOTIFYICONDATA);

	// Bind the NOTIFYICONDATA structure to our global hwnd ( handle to main window ).
	g_notifyIconData.hWnd = hwnd;

	// Set the NOTIFYICONDATA ID. HWND and uID form a unique identifier for each item in system tray.
	g_notifyIconData.uID = ID_TRAY_APP_ICON;

	// Set up flags.
	g_notifyIconData.uFlags = NIF_ICON | // Guarantees that the hIcon member will be a valid icon.
		                      NIF_MESSAGE | // When someone clicks in the system tray icon, we want a WM_ type message to be sent to our WNDPROC
		                      NIF_TIP  | NIF_INFO| NIF_SHOWTIP; // Show tooltip.

	// This message must be handled in hwnd's window procedure.
	g_notifyIconData.uCallbackMessage = WM_TRAYICON;

	// Load the icon as a resource.
	g_notifyIconData.hIcon = LoadIcon(hINSTANCE,MAKEINTRESOURCE(IDI_ICON1));

	// Set the tooltip text.
	wcscpy_s(g_notifyIconData.szTip, L"KA Lite");

	// Time to display the tooltip.
	//g_notifyIconData.uTimeout = 100;

	// Type of tooltip (balloon).
	g_notifyIconData.dwInfoFlags = NIIF_INFO;

	// Copy text to the structure.
	lstrcpy(g_notifyIconData.szInfo , L"Right click for KA Lite options!");
	lstrcpy(g_notifyIconData.szInfoTitle , L"KA Lite");
}

// The name of the window class.
LPCWSTR g_szClassName = L"KA Lite Class";

// Function to handle the messages from the "about" dialog.
LRESULT CALLBACK AboutDialogProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		case WM_INITDIALOG:
			return TRUE;
		case WM_COMMAND:
			switch(wParam)
			{
			case IDOK:
				EndDialog(hwnd, IDOK);
				break;
			case IDCANCEL:
				EndDialog(hwnd, IDCANCEL);
				break;
			}
			break;
		default:
			return FALSE;
	}
	return TRUE;
}

// Main window function that processes the messages that come to the window.
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CheckMenus();
	
	if( msg==WM_TASKBARCREATED && !IsWindowVisible(hwnd))
	{
		Minimize();
		return 0;
	}

	switch(msg)
	{

		case WM_PAINT:
		{
			HDC hDC, MemDCLogo;
			PAINTSTRUCT Ps;
			HBITMAP bmp1;
 
			hDC = BeginPaint(hwnd, &Ps);

			bmp1 = LoadBitmap(hINSTANCE,MAKEINTRESOURCE(IDB_BITMAP2));

			if(bmp1 == NULL)
				MessageBox(hwnd, L"Image not loaded" , L"Image Error", MB_OK | MB_ICONINFORMATION);

			MemDCLogo = CreateCompatibleDC(hDC);
			if(MemDCLogo == NULL)
				MessageBox(hwnd, L"HDC error" , L"HDC Error", MB_OK | MB_ICONINFORMATION);


			SelectObject(MemDCLogo, bmp1);
			BitBlt(hDC, 3, 3, 261, 66, MemDCLogo, 0, 0, SRCCOPY); 
			DeleteDC(MemDCLogo);
			DeleteObject(bmp1);			
			EndPaint(hwnd, &Ps);
		}
		break;

	case WM_CREATE:
		{
			HICON hIcon, hIconSm;
			
			hMenu = CreatePopupMenu();
			
			if(SERVERISRUNNING)
			{
				AppendMenu(hMenu, MF_STRING, ID_TRAY_START_STOP_CONTEXT_MENU_ITEM, L"Stop server");
			} else {
				AppendMenu(hMenu, MF_STRING, ID_TRAY_START_STOP_CONTEXT_MENU_ITEM, L"Start server");
			}						
			AppendMenu(hMenu, MF_STRING, ID_TRAY_BROWSER_CONTEXT_MENU_ITEM, L"Open KA Lite");
			AppendMenu(hMenu, MF_STRING, ID_TRAY_RESTORE_CONTEXT_MENU_ITEM, L"Restore window");
			AppendMenu(hMenu, MF_STRING, ID_TRAY_EXIT_CONTEXT_MENU_ITEM, L"Exit KA Lite");				
			
			hMainMenu = CreateMenu();
			hFileSubMenu = CreatePopupMenu();
			AppendMenu(hFileSubMenu, MF_STRING, ID_FILE_EXIT, L"E&xit");
			AppendMenu(hMainMenu, MF_STRING | MF_POPUP, (UINT)hFileSubMenu, L"&File");

			hOptionsSubMenu = CreatePopupMenu();
			AppendMenu(hOptionsSubMenu, MF_STRING, ID_OPTIONS_RUNSTARTUP, L"Run KA Lite at system startup");
			AppendMenu(hOptionsSubMenu, MF_STRING, ID_OPTIONS_AUTOMINIMIZE, L"Minimize to tray when KA Lite is run");
			AppendMenu(hOptionsSubMenu, MF_STRING, ID_OPTIONS_AUTOSTART, L"Auto-start server when KA Lite is run");
			AppendMenu(hMainMenu, MF_STRING | MF_POPUP, (UINT)hOptionsSubMenu, L"Options");

			hAboutSubMenu = CreatePopupMenu();
			AppendMenu(hAboutSubMenu, MF_STRING, ID_HELP_ABOUT, L"&About");
			AppendMenu(hMainMenu, MF_STRING | MF_POPUP, (UINT)hAboutSubMenu, L"&Help");

			SetMenu(hwnd, hMainMenu);

			hIcon = LoadIcon(hINSTANCE,MAKEINTRESOURCE(IDI_ICON1));
			if(hIcon)
				SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
			else
				MessageBox(hwnd, L"Failed to load the image.", L"Error", MB_OK | MB_ICONERROR);

			hIconSm = LoadIcon(hINSTANCE,MAKEINTRESOURCE(IDI_ICON1));
			if(hIcon)
				SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
			else
				MessageBox(hwnd, L"Failed to load the image.", L"Error", MB_OK | MB_ICONERROR); 

			CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"Start", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 83, 120, 30, hwnd, (HMENU) ID_START_STOP_BUTTON,(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
			CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON", L"Open KA lite",  WS_TABSTOP |WS_VISIBLE|WS_CHILD | BS_DEFPUSHBUTTON, 140, 83, 120,30, hwnd, (HMENU) ID_OPEN_IN_BROWSER, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
			
			if (startupCheckState == SW_SHOWNA) 
			{
				CheckMenuItem(hOptionsSubMenu, ID_OPTIONS_RUNSTARTUP, MF_CHECKED);
			} else {
				CheckMenuItem(hOptionsSubMenu, ID_OPTIONS_RUNSTARTUP, MF_UNCHECKED);
			}

			if (autoMinimizeCheckState == SW_SHOWNA)
			{
				CheckMenuItem(hOptionsSubMenu, ID_OPTIONS_AUTOMINIMIZE, MF_CHECKED);
				EnableMenuItem(hOptionsSubMenu, ID_OPTIONS_AUTOSTART, MF_ENABLED);
			} else {
				CheckMenuItem(hOptionsSubMenu, ID_OPTIONS_AUTOMINIMIZE, MF_UNCHECKED);
				EnableMenuItem(hOptionsSubMenu, ID_OPTIONS_AUTOSTART, MF_DISABLED | MF_GRAYED);
			}

			if (autoStartServer == SW_SHOWNA)
			{
				CheckMenuItem(hOptionsSubMenu, ID_OPTIONS_AUTOSTART, MF_CHECKED);
			} else {
				CheckMenuItem(hOptionsSubMenu, ID_OPTIONS_AUTOSTART, MF_UNCHECKED);
			}

			CHANGEDSTATE = true;
		}
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			case ID_START_BUTTON:
				prepareShellInfoStructures();
				startServerCommand();
				break;
			case ID_STOP_BUTTON:
				stopServerCommand();
				break;
			case ID_START_STOP_BUTTON:
				if(SERVERISRUNNING)
				{
					stopServerCommand();
					SetDlgItemTextW(hwnd, ID_START_STOP_BUTTON, L"Start server");
					CHANGEDSTATE = true;
				} else {
					prepareShellInfoStructures();
					startServerCommand();
					SetDlgItemTextW(hwnd, ID_START_STOP_BUTTON, L"Stop server");
					CHANGEDSTATE = true;
				}				
				break;
			case ID_MINIMIZE_BUTTON:
				Minimize();
				break;
			case ID_OPEN_IN_BROWSER:
				if((int)ShellExecute(hwnd,L"open",L"http://127.0.0.1:8008/",NULL,NULL,SW_MAXIMIZE) <= 36)
				{
					MessageBox(hwnd,L"Cannot open KA Lite in browser!", L"Opening KA Lite error", MB_OK | MB_ICONINFORMATION);
				}
				break;
			case ID_OPTIONS_RUNSTARTUP:
				startupCheckState = GetMenuState(hOptionsSubMenu, ID_OPTIONS_RUNSTARTUP, MF_BYCOMMAND);

                if (startupCheckState == SW_SHOWNA) {
					CheckMenuItem(hOptionsSubMenu, ID_OPTIONS_RUNSTARTUP, MF_UNCHECKED);
					ReadBuffer[0] = '1';
					setConfigFile(ReadBuffer);
					refreshConfigBuffer();
					setStartupShortcut(1);
                } else {
                    CheckMenuItem(hOptionsSubMenu, ID_OPTIONS_RUNSTARTUP, MF_CHECKED);
					ReadBuffer[0] = '0';
					setConfigFile(ReadBuffer);
					refreshConfigBuffer();
					setStartupShortcut(0);
                }				  
				break;
			case ID_OPTIONS_AUTOMINIMIZE:
				autoMinimizeCheckState = GetMenuState(hOptionsSubMenu, ID_OPTIONS_AUTOMINIMIZE, MF_BYCOMMAND); 
                if (autoMinimizeCheckState == SW_SHOWNA) {
					CheckMenuItem(hOptionsSubMenu, ID_OPTIONS_AUTOMINIMIZE, MF_UNCHECKED);
					CheckMenuItem(hOptionsSubMenu, ID_OPTIONS_AUTOSTART, MF_UNCHECKED);
					EnableMenuItem(hOptionsSubMenu, ID_OPTIONS_AUTOSTART, MF_DISABLED | MF_GRAYED);
					ReadBuffer[1] = '1';
					ReadBuffer[2] = '1';
					setConfigFile(ReadBuffer);
					refreshConfigBuffer();
                } else {
                    CheckMenuItem(hOptionsSubMenu, ID_OPTIONS_AUTOMINIMIZE, MF_CHECKED);
					EnableMenuItem(hOptionsSubMenu, ID_OPTIONS_AUTOSTART, MF_ENABLED);
					ReadBuffer[1] = '0';
					setConfigFile(ReadBuffer);
					refreshConfigBuffer();
                }
				break;
			case ID_OPTIONS_AUTOSTART:
				if (autoStartServer == SW_SHOWNA)
				{
					CheckMenuItem(hOptionsSubMenu, ID_OPTIONS_AUTOSTART, MF_UNCHECKED);
					ReadBuffer[2] = '1';
					setConfigFile(ReadBuffer);
					refreshConfigBuffer();
				} else {
					CheckMenuItem(hOptionsSubMenu, ID_OPTIONS_AUTOSTART, MF_CHECKED);
					ReadBuffer[2] = '0';
					setConfigFile(ReadBuffer);
					refreshConfigBuffer();
				}
				break;
			case ID_HELP_ABOUT:
				{
					int ret = DialogBox(hINSTANCE, MAKEINTRESOURCE(IDD_DIALOG1), hwnd,reinterpret_cast<DLGPROC>( AboutDialogProc));
				}
				break;
			case ID_FILE_EXIT:
				if(MessageBox(hwnd, L"Are you sure you want to exit from KA Lite?", L"KA Lite Exit", MB_YESNO | MB_ICONQUESTION) == IDYES)
				{
					stopServerCommand();
					PostQuitMessage(0);
				}				
				break;
			case ID_STUFF_GO:
				break;
		}
		break;

	case WM_TRAYICON:
    {
		  printf( "Tray icon notification, from %d\n", wParam ) ;
      
		  switch(wParam)
		  {
		  case ID_TRAY_APP_ICON:
			printf( "Its the ID_TRAY_APP_ICON. One app can have several tray icons.\n" ) ;
			break;
		  }

		  // React when the mouse button is released.
		  if (lParam == WM_LBUTTONUP)
		  {
			// The window is restored.
			Restore();
		  }
		  else if (lParam == WM_RBUTTONDOWN) // I'm using WM_RBUTTONDOWN here because
		  {
			// Show the context menu.
			// Get current mouse position.
			POINT curPoint ;
			GetCursorPos( &curPoint ) ;
        
			// Should SetForegroundWindow according to original poster so the popup shows on top.
			SetForegroundWindow(hwnd);        
        
			// TrackPopupMenu blocks the application until TrackPopupMenu returns.
			UINT clicked = TrackPopupMenu(
          
			  hMenu,
			  TPM_RETURNCMD | TPM_NONOTIFY, // Don't send WM_COMMAND messages about this window, instead return the identifier of the clicked menu item.
			  curPoint.x,
			  curPoint.y,
			  0,
			  hwnd,
			  NULL

			);

			// Exit from KA Lite.
			if (clicked == ID_TRAY_EXIT_CONTEXT_MENU_ITEM)
			{				
				if(MessageBox(hwnd, L"Are you sure you want to exit from KA Lite?", L"KA Lite Exit", MB_YESNO | MB_ICONQUESTION) == IDYES)
				{
					stopServerCommand();
					PostQuitMessage(0);
				}
			} 

			// Starts the server.
			if(clicked == ID_TRAY_START_CONTEXT_MENU_ITEM)
			{
				prepareShellInfoStructures();
				startServerCommand();
			}

			// Stop the server.
			if(clicked == ID_TRAY_STOP_CONTEXT_MENU_ITEM)
			{
				stopServerCommand();
			}

			if(clicked == ID_TRAY_START_STOP_CONTEXT_MENU_ITEM)
			{
				if(SERVERISRUNNING)
				{
					stopServerCommand();
					hMenu = CreatePopupMenu();
					AppendMenu(hMenu, MF_STRING, ID_TRAY_START_STOP_CONTEXT_MENU_ITEM, L"Start server");											
					AppendMenu(hMenu, MF_STRING, ID_TRAY_BROWSER_CONTEXT_MENU_ITEM, L"Open KA Lite");
					AppendMenu(hMenu, MF_STRING, ID_TRAY_RESTORE_CONTEXT_MENU_ITEM, L"Restore window");
					AppendMenu(hMenu, MF_STRING, ID_TRAY_EXIT_CONTEXT_MENU_ITEM, L"Exit KA Lite");
					CHANGEDSTATE = true;
				} else {
					prepareShellInfoStructures();
					startServerCommand();
					hMenu = CreatePopupMenu();
					AppendMenu(hMenu, MF_STRING, ID_TRAY_START_STOP_CONTEXT_MENU_ITEM, L"Stop server");											
					AppendMenu(hMenu, MF_STRING, ID_TRAY_BROWSER_CONTEXT_MENU_ITEM, L"Open KA Lite");
					AppendMenu(hMenu, MF_STRING, ID_TRAY_RESTORE_CONTEXT_MENU_ITEM, L"Restore window");
					AppendMenu(hMenu, MF_STRING, ID_TRAY_EXIT_CONTEXT_MENU_ITEM, L"Exit KA Lite");
					CHANGEDSTATE = true;
				}
			}

			// Open in browser.
			if(clicked == ID_TRAY_BROWSER_CONTEXT_MENU_ITEM)
			{
			  if((int)ShellExecute(hwnd,L"open",L"http://127.0.0.1:8008/",NULL,NULL,SW_MAXIMIZE) <= 36)
				{
					MessageBox(hwnd,L"Cannot open KA Lite in browser!", L"Opening KA Lite error", MB_OK | MB_ICONINFORMATION);
				}
			}

			// Restore window.
			if(clicked == ID_TRAY_RESTORE_CONTEXT_MENU_ITEM)
			{
				Restore();
			}
		  }		  
    }
    break;

	case WM_NCHITTEST:
		{
			// This tests if you are not in the client area. (Out of the window).
			UINT uHitTest = DefWindowProc(hwnd, WM_NCHITTEST, wParam, lParam);
			if(uHitTest == HTCLIENT)
			  return HTCAPTION;
			else
			  return uHitTest;
		}		
	
	case WM_CLOSE:
		Minimize() ;
		return 0;
		break;
	case WM_DESTROY:
		if(SERVERISRUNNING)
		{
			stopServerCommand();
		}		
		PostQuitMessage(0);
		break;
	
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

TCHAR className[] = L"KA Lite Class";

HWND GetRunningWindow()
{
	// Check if exists an application with the same class name as this application
	HWND hWnd = FindWindow(className, NULL);
	if (IsWindow(hWnd))
	{
		HWND hWndPopup = GetLastActivePopup(hWnd);
		if (IsWindow(hWndPopup))
			hWnd = hWndPopup; // Previous instance exists
	}
	else {
		hWnd = NULL; // Previous instance doesnt exist
		return hWnd;
	}
	return hWnd;
}


// Main application window.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	
	// Checks for previous instance of our program
	HWND hOtherWnd = GetRunningWindow();

	// Allow only one instance of the program
	if (hOtherWnd) // hOtherWnd != NULL -> Previous instance exists
	{
		MessageBox(hOtherWnd, L"KA Lite is already running", L"Info", MB_OK);
		SetForegroundWindow(hOtherWnd); // Activate it &
		if (IsIconic(hOtherWnd))
		ShowWindow(hOtherWnd, SW_RESTORE); // restore it
		return FALSE; // Exit program
	}

	TCHAR windowTitle[30] = {};
	wcscat_s(windowTitle, L"KA Lite ");
	
	if(getVersionFromFile()==0)
	{		
		wcscat_s(windowTitle, versionFinal);
	}

	Sleep(5000);

	runServerOnCreationMessage = TRUE;
	CHANGEDSTATE = FALSE;
	SERVERISRUNNING = FALSE;	

	WM_TASKBARCREATED = RegisterWindowMessageA("TaskbarCreated");

	hINSTANCE = hInstance;
	WNDCLASSEX wc = {0};
	MSG Msg;
	
	// Registering the window class.
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hINSTANCE,MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE;
	wc.lpszClassName = className;
	wc.hIconSm = LoadIcon(hINSTANCE,MAKEINTRESOURCE(IDI_ICON1));
	wc.style=CS_HREDRAW|CS_VREDRAW;

	if(!RegisterClassEx(&wc)){
		MessageBox(NULL, L"Failed to regist window.", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	if(readConfigFile() == 0)
	{
		if ( ReadBuffer[0] == '0' )
		{
			setStartupShortcut(0);
			startupCheckState = SW_SHOWNA;
		}
		else
		{
			setStartupShortcut(1);
		}		
		if( ReadBuffer[1] == '0' )
		{
			autoMinimizeCheckState = SW_SHOWNA;
		}		
		if( ReadBuffer[2] == '0' )
		{
			autoStartServer = SW_SHOWNA;
			prepareShellInfoStructures();
		    startServerCommand();
		}
	}
	
	// Creating the window.
	DWORD windowStyle = WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU| CS_NOCLOSE;
	hwnd = CreateWindowEx(NULL, className, windowTitle, windowStyle , CW_USEDEFAULT, CW_USEDEFAULT, 285, 180, NULL,  NULL, hInstance, NULL);	

	if(hwnd == NULL){
		MessageBox(NULL, L"Failed to create the window.", L"Error",MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	
	SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_MINIMIZEBOX);
	SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);

	// Initialize the NOTIFYICONDATA structure.
	InitNotifyIconData();

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);	

	if(autoMinimizeCheckState == SW_SHOWNA)
	{
		if(SERVERISRUNNING)
		{
			lstrcpy(g_notifyIconData.szInfoTitle , L"KA Lite is running.");
			lstrcpy(g_notifyIconData.szInfo , L"The server should now be accessible locally at: http://127.0.0.1:8008/ or you can press \"Open KA Lite button\"");
			Minimize();
			lstrcpy(g_notifyIconData.szInfoTitle , L"KA Lite");
			lstrcpy(g_notifyIconData.szInfo , L"Right click for KA Lite options!");
		}
		else 
		{
			g_notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_SHOWTIP;
			Minimize();
		}
		InitNotifyIconData();
	}

	runServerOnCreationMessage = FALSE;

	// The Message Loop.
	while(GetMessage(&Msg, NULL, 0 , 0) > 0){
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	if(!IsWindowVisible(hwnd))
	{
		Shell_NotifyIcon(NIM_DELETE, &g_notifyIconData);
	}

	return Msg.wParam;
}

