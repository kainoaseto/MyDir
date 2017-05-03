#include "mydir.h"

int list_directories(char* path, BOOL longName)
{
	ULONG				filesize;
	char*				trim;
	HANDLE				hFind;
	WIN32_FIND_DATA		found;
	char				pattern[MAX_PATH];
	char				origPath[MAX_PATH];
	SYSTEMTIME          stUTC, stLocal;

	printf("Path: %s\n", path);
	GetCurrentDirectory(MAX_PATH, origPath);

	if (SetCurrentDirectory(path)) {
		strcat_s(path, MAX_PATH, "\\");
		SetCurrentDirectory(origPath);
		
	}

	strcpy_s(pattern, MAX_PATH, path);

	// Auto trim path to remove any file  or wildcard reference
	if (strstr(path, "*") != NULL || strstr(path, "?") != NULL)
	{ 
		trim = strrchr(path, '\\');
		if (trim != NULL)
		{
			trim++;
			*trim = '\0';
		}
	}
	

	// Check if the directory exists
	if (file_exists(path))
	{
		if (strcmp(path, "") != 0 && !SetCurrentDirectory(path))
		{
			printf("Error: The directory specified does not exist\n");
			exit(1);
		}
	}
	

	GetCurrentDirectory(MAX_PATH, path);

	// Check if we need to add a \ to the end of the path
	if (strcmp(path, "") != 0 &&
		path[strlen(path) - 1] != ':' &&
		path[strlen(path) - 1] != '\\' &&
		directory_exists(path))
	{
		strcat_s(path, MAX_PATH, "\\");
	}

	printf("NewPath: %s\n", path);

	if (directory_exists(pattern))
	{
		strcat_s(pattern, MAX_PATH, "\\");
	}

	if ((strstr(path, "*") == NULL || strstr(path, "?") == NULL) &&
		!file_exists(path))
	{
		strcat_s(pattern, MAX_PATH, "*.*");
	}

	printf("Pattern: %s\n", pattern);
	hFind = FindFirstFile(pattern, &found);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			if (strcmp(found.cFileName, ".") != 0 &&
				strcmp(found.cFileName, "..") != 0)
			{
				if (longName)
				{
					filesize = (found.nFileSizeHigh * (MAXDWORD + 1)) + found.nFileSizeLow;
					//found.dwFileAttributes;
					FileTimeToSystemTime(&found.ftLastWriteTime, &stUTC);
					SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

					printf("%d-%02d-%02d  %02d:%02d    %10u        %s", 
						stLocal.wYear, stLocal.wMonth, stLocal.wDay,
						stLocal.wHour, stLocal.wMinute, filesize, path);
				}
				printf("%s\n", found.cFileName);
			}
		} while (FindNextFile(hFind, &found));
		FindClose(hFind);
	}

	return 0;
}

BOOL directory_exists(char* path)
{
	DWORD dwAttrib = GetFileAttributes(path);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
			(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

BOOL file_exists(char* path)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE handle = FindFirstFile(path, &FindFileData);
	BOOL found = handle != INVALID_HANDLE_VALUE;
	if (found)
	{
		//FindClose(&handle); this will crash
		FindClose(handle);
	}
	return found;
}