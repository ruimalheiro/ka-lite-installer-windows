#ifndef version
#define version


/*************************************************************************************
*                                                                                    *
*	Functions declaration.                                                           *
*                                                                                    *
**************************************************************************************/
int getVersionFromFile(TCHAR * versionFinal);
void setKALiteVersion(TCHAR * windowTitle, int windowTitleSize);



/*************************************************************************************
*                                                                                    *
*	This function extracts KALite version from "version.py".                         *
*                                                                                    *
**************************************************************************************/
int getVersionFromFile(TCHAR * versionFinal)
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



/*************************************************************************************
*                                                                                    *
*	This function sets the title of KA Lite window with the version.                 *
*                                                                                    *
**************************************************************************************/
void setKALiteVersion(TCHAR * windowTitle, int windowTitleSize)
{
	TCHAR versionFinal[MAX_PATH] = {};
	wcscat_s(windowTitle, windowTitleSize, L"KA Lite ");

	if(getVersionFromFile(versionFinal)==0)
	{		
		wcscat_s(windowTitle, windowTitleSize, versionFinal);
	}
}

#endif