#include "fle_win32_framework.h"


HANDLE ghMutex;
static bool isrunning;

// Declare global stuff that you need to use inside the functions.
fle_TrayWindow * window;

fle_TrayMenuItem * menu1;
fle_TrayMenuItem * menu2;
fle_TrayMenuItem * menu3;
fle_TrayMenuItem * menu4;
fle_TrayMenuItem * menu5;
fle_TrayMenuItem * menu6;
fle_TrayMenuItem * menu7;


void trayMenuAction1()
{
	if(!runShellScript("start.bat", NULL, "ka-lite\\"))
	{
		// Handle error.
		printConsole("Failed to run the script.\n");
	}
	else
	{
		menu1->disable();
		menu2->enable();
		printConsole("The script was run successfully.\n");
	}
}

void trayMenuAction2()
{
	if(!runShellScript("stop.bat", NULL, "ka-lite\\"))
	{
		// Handle error.
		printConsole("Failed to run the script.\n");
	}
	else
	{
		menu2->disable();
		menu1->enable();
		printConsole("The script was run successfully.\n");
	}
}

void trayMenuAction3()
{
	if(menu4->isChecked())
	{
		menu4->uncheck();
		menu5->enable();
	}
	else
	{
		menu4->check();
		menu5->disable();
	}
}

void trayMenuAction4()
{
	//printConsole("I am exiting KA Lite.\n");
	menu1->toogleEnabled();
	//menu1->disable();
}

void doStuff()
{
	if(isrunning)
	{
		menu2->enable();
	}
	else
	{
		menu2->disable();
	}
}

void statusFunction()
{
	// The code here runs on each frame of the window main loop.
	// We can handle things like checking if the server is online and controlling the state of each component.
	// Basically, the automated stuff.
	handleMutex(&ghMutex, 1000, &doStuff);
}

void doStuffThread()
{
	isrunning = !isrunning;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	isrunning = false;
	ghMutex = CreateMutex(NULL, FALSE, NULL);
	if (ghMutex == NULL) 
    {
        return 1;
    }
	
	ThreadLoopHandleFunction(&ghMutex, 3000, &doStuffThread);

	window = new fle_TrayWindow(&hInstance);
	window->setTrayIcon("images\\logo48.ico");
	window->setStatusFunction(&statusFunction);

	menu1 = new fle_TrayMenuItem("Start Server.", &trayMenuAction1);
	menu2 = new fle_TrayMenuItem("Stop Server.", &trayMenuAction2);
	menu3 = new fle_TrayMenuItem("Options", NULL);
	menu4 = new fle_TrayMenuItem("Run KA Lite when the user logs in.", &trayMenuAction3);
	menu5 = new fle_TrayMenuItem("Run KA Lite at system startup.", NULL);
	menu6 = new fle_TrayMenuItem("Auto-start server when KA Lite is run.", NULL);
	menu7 = new fle_TrayMenuItem("Exit KA Lite.", &trayMenuAction4);

	menu3->setSubMenu();
	menu3->addSubMenu(menu4);
	menu3->addSubMenu(menu5);
	menu3->addSubMenu(menu6);
	
	window->addMenu(menu1);
	window->addMenu(menu2);
	window->addMenu(menu3);
	window->addMenu(menu7);

	window->show();

	CloseHandle(ghMutex);	
	return 0;
}

