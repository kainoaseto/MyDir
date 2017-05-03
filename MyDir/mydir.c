#include "mydir.h"

int list_directories(char* path, BOOL longName)
{
	ULONG				filesize;
	char*				trim;
	HANDLE				hFind;
	WIN32_FIND_DATA		found;
	char				pattern[MAX_PATH];
	SYSTEMTIME          stUTC, stLocal;
	DWORD				attr;

	if (strcmp(path, "") != 0 &&
		path[strlen(path) - 1] != ':' &&
		path[strlen(path) - 1] != '\\' &&
		SetCurrentDirectory(path)) 
	{
		strcpy_s(pattern, MAX_PATH, "*.*");
	}
	else
	{
		strcpy_s(pattern, MAX_PATH, path);

		if (!file_exists(pattern) && 
				(
					(!directory_exists(pattern) && pattern[strlen(pattern) - 1] != '\\' ) ||
					( strstr(pattern, "*") == NULL && strstr(pattern, "?") == NULL )
				)
		   )
		{
			strcat_s(pattern, MAX_PATH, "*.*");
		}

		// Check if any partial directories or files exist
		if (!file_exists(pattern) && !directory_exists(pattern))
		{
			if (strcmp(path, "") != 0 && !SetCurrentDirectory(path))
			{
				printf("Error: The directory specified does not exist\n");
				exit(1);
			}
		}
	}

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
					attr = found.dwFileAttributes;
					FileTimeToSystemTime(&found.ftLastWriteTime, &stUTC);
					SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

					printf("%d-%02d-%02d  %02d:%02d    %10u    [%c%c%c%c]   ", 
						stLocal.wYear, stLocal.wMonth, stLocal.wDay,
						stLocal.wHour, stLocal.wMinute, filesize, 
						(attr & FILE_ATTRIBUTE_ARCHIVE) ? 'A' : ' ',
						(attr & FILE_ATTRIBUTE_SYSTEM) ? 'S' : ' ',
						(attr & FILE_ATTRIBUTE_HIDDEN) ? 'H' : ' ',
						(attr & FILE_ATTRIBUTE_READONLY) ? 'R' : ' ');
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
	WIN32_FIND_DATA data;
	HANDLE handle = FindFirstFile(path, &data);
	BOOL found = (handle != INVALID_HANDLE_VALUE &&
		(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY));
	if (found)
	{
		FindClose(handle);
	}
	return found;
}

BOOL file_exists(char* path)
{
	WIN32_FIND_DATA data;
	HANDLE handle = FindFirstFile(path, &data);
	BOOL found = (handle != INVALID_HANDLE_VALUE &&
					!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY));
	if (found)
	{
		FindClose(handle);
	}
	return found;
}