#include "fle_win32_framework.h"

// Declare global stuff that you need to use inside the functions.
fle_TrayWindow * window;

fle_TrayMenuItem * menu1;
fle_TrayMenuItem * menu2;
fle_TrayMenuItem * menu3;
fle_TrayMenuItem * menu4;
fle_TrayMenuItem * menu5;
fle_TrayMenuItem * menu6;
fle_TrayMenuItem * menu7;


void startServerAction()
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

void stopServerAction()
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

void exitKALiteAction()
{
	if(ask("Exiting..." , "Really want to exit KA Lite?"))
	{
		window->quit();
	}
}

void doStuffThread()
{
	// We can handle things like checking if the server is online and controlling the state of each component.
	if(isServerOnline("KA Lite session", "http://127.0.0.1:8008/"))
	{
		menu1->disable();
		menu2->enable();
		window->sendTrayMessage("KA LITE", "Server is running!!!");
	}
	else
	{
		menu1->enable();
		menu2->disable();
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	startThread(NULL, TRUE, 3000, &doStuffThread);

	window = new fle_TrayWindow(&hInstance);
	window->setTrayIcon("images\\logo48.ico");

	menu1 = new fle_TrayMenuItem("Start Server.", &startServerAction);
	menu2 = new fle_TrayMenuItem("Stop Server.", &stopServerAction);
	menu3 = new fle_TrayMenuItem("Options", NULL);
	menu4 = new fle_TrayMenuItem("Run KA Lite when the user logs in.", NULL);
	menu5 = new fle_TrayMenuItem("Run KA Lite at system startup.", NULL);
	menu6 = new fle_TrayMenuItem("Auto-start server when KA Lite is run.", NULL);
	menu7 = new fle_TrayMenuItem("Exit KA Lite.", &exitKALiteAction);

	menu3->setSubMenu();
	menu3->addSubMenu(menu4);
	menu3->addSubMenu(menu5);
	menu3->addSubMenu(menu6);
	
	window->addMenu(menu1);
	window->addMenu(menu2);
	window->addMenu(menu3);
	window->addMenu(menu7);

	menu2->disable();

	window->show();

	return 0;
}

