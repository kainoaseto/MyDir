// Kainoa Seto
// MyDir OS program that replicates the basic functionality of the MSDOS DIR
// This file contains all the implementation of the program
// 5/4/17
// mydir.c
#include "mydir.h"

int list__dir(char* path, BOOL longName)
{
	ULONG				filesize;			// Filesize of current item found
	HANDLE				hFind;				// File handle of current item found
	WIN32_FIND_DATA		found;				// File/dir data and attributes if found
	char				pattern[MAX_PATH];	// Search pattern based on path input
	SYSTEMTIME          stUTC, stLocal;		// Different times for fomratting of longname
	DWORD				attr;				// attributes of current file found

	// If we are looking for a file/dir and have given input that is not the current directory
	// and that we can navigate to without any issue, just print out everything in the directory
	if (strcmp(path, "") != 0 &&
		path[strlen(path) - 1] != ':' &&
		SetCurrentDirectory(path)) 
	{
		strcpy_s(pattern, MAX_PATH, "*.*");
	}
	else
	{
		// Copy the pattern from the path since it may contain wild cards already
		strcpy_s(pattern, MAX_PATH, path);

		if (!file_exists(pattern) && !directory_exists(pattern))
		{
			if (strcmp(path, "") != 0 && !SetCurrentDirectory(path))
			{
				printf("Error: The file does not exist\n");
				return 1;
			}

			strcat_s(pattern, MAX_PATH, "*.*");
		}
	}

	// Find the first file and as long as we find a file, start the search loop
	hFind = FindFirstFile(pattern, &found);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		// Search loop
		do {
			// Exclude the directory shortcuts
			if (strcmp(found.cFileName, ".") != 0 &&
				strcmp(found.cFileName, "..") != 0)
			{
				// If it sa longname print out the filesize, attributes, and last File Write time in local system time
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
				// Always print the filename/dirname
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
	// Make sure we found a valid find and that it is a directory
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
	// If we found a valid find and it is not a directory then it must be a file
	BOOL found = (handle != INVALID_HANDLE_VALUE &&
					!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY));
	if (found)
	{
		FindClose(handle);
	}
	return found;
}