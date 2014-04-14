#ifndef version
#define version


/*
*	Functions declaration.
*/
int getVersionFromFile(TCHAR * versionFinal);
void setKALiteVersion(TCHAR * windowTitle, int windowTitleSize);



/*
*	This function extracts KALite version from "version.py".
*/
int getVersionFromFile(TCHAR * versionFinal)
{
	HANDLE hFile;
	DWORD bytesRead = 0;

	hFile = CreateFile(L"ka-lite\\kalite\\version.py", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		// Fails to open the file but continues and create one.
		CloseHandle(hFile);
		return 1;
	}

	LARGE_INTEGER fileSize = { 0 };
	if(GetFileSizeEx(hFile, &fileSize) == 0)
	{
		MessageBox(NULL, L"Failed to get the size of the file.", L"Error", MB_OK | MB_ICONERROR);
		CloseHandle(hFile);
		return 1;
	}

	int SIZE = int(fileSize.LowPart);
	char * versionBuffer = new char[SIZE];

	if(ReadFile(hFile, versionBuffer, SIZE, &bytesRead, NULL) == FALSE)
	{
		MessageBox(NULL, L"Failed to read the config file", L"Error", MB_OK | MB_ICONERROR);
		CloseHandle(hFile);
		return 1;
	}

	if(bytesRead == SIZE)
	{
		bool SEARCHING = TRUE;
		bool READ_VERSION_FLAG = FALSE;
		int INDEX = 0;
		for (INDEX = 0; INDEX < SIZE; INDEX++)
		{
			if(SEARCHING)
			{
				if((INDEX+7) <= (SIZE-1))
				{
					if ((versionBuffer[INDEX] == 'V') &&
						(versionBuffer[INDEX + 1] == 'E') &&
						(versionBuffer[INDEX + 2] == 'R') &&
						(versionBuffer[INDEX + 3] == 'S') &&
						(versionBuffer[INDEX + 4] == 'I') &&
						(versionBuffer[INDEX + 5] == 'O') &&
						(versionBuffer[INDEX + 6] == 'N'))
					{
						READ_VERSION_FLAG = TRUE;
					}

					int CURRENT_INDEX = INDEX + 7;
					int TEMP_INDEX = 0;
					if (READ_VERSION_FLAG)
					{
						while(versionBuffer[CURRENT_INDEX] != '\n')
						{
							if((versionBuffer[CURRENT_INDEX] != ' ') &&
							   (versionBuffer[CURRENT_INDEX] != '"') &&
							   (versionBuffer[CURRENT_INDEX] != '='))
							{
								versionFinal[TEMP_INDEX] = versionBuffer[CURRENT_INDEX];
								TEMP_INDEX++;
							}
							CURRENT_INDEX++;
						}
						READ_VERSION_FLAG = FALSE;
						SEARCHING = FALSE;
						versionFinal[TEMP_INDEX] = '\0';
						break;
					}
				}
			}
		}
		CloseHandle(hFile);
		return 0;
	}
	else if(bytesRead == 0)
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



/*
*	This function sets the title of KA Lite window with the version.
*/
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