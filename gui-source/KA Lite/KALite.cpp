#include "fle_win32_framework.h"

fle_TrayWindow * window;

fle_TrayMenuItem * menu1;
fle_TrayMenuItem * menu2;
fle_TrayMenuItem * menu3;
fle_TrayMenuItem * menu4;
fle_TrayMenuItem * menu5;


void trayMenuAction1()
{
	/*if(!runShellScript(L"start.bat", NULL, L"ka-lite\\"))
	{
		// Handle error.
	}*/
	menu2->check();
}

void trayMenuAction2()
{
	/*if(!runShellScript(L"stop.bat", NULL, L"ka-lite\\"))
	{
		// Handle error.
	}*/
	menu1->disable();
}

void trayMenuAction3()
{
	printConsole(L"I am an option.\n");
}

void trayMenuAction4()
{
	printConsole(L"I am exiting KA Lite.\n");
}

void statusFunction()
{
	// The code here runs on each frame of the window main loop.
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	window = new fle_TrayWindow(&hInstance);
	window->setTrayIcon(L"images\\logo48.ico");
	window->setStatusFunction(&statusFunction);

	menu1 = new fle_TrayMenuItem(L"Start Server", &trayMenuAction1);
	menu2 = new fle_TrayMenuItem(L"Stop Server", &trayMenuAction2);
	menu3 = new fle_TrayMenuItem(L"Options", NULL);
	menu4 = new fle_TrayMenuItem(L"Option 1", &trayMenuAction3);
	menu5 = new fle_TrayMenuItem(L"Exit KA Lite", &trayMenuAction4);

	menu3->setSubMenu();
	menu3->addSubMenu(menu4);
	
	window->addMenu(menu1);
	window->addMenu(menu2);
	window->addMenu(menu3);
	window->addMenu(menu5);

	window->show();

	return 0;
}

