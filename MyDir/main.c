#include "mydir.h"

int main(int argc, char *argv[])
{
	BOOL		longName;
	char		path[MAX_PATH];

	longName = FALSE;
	strcpy_s(path, MAX_PATH, "");

	int i;
	for (i = 1; i < argc; i++)
	{
		// Check if we need to print out help and then exit
		if (stricmp(argv[i], "/?") == 0 ||
			stricmp(argv[i], "--help") == 0 ||
			stricmp(argv[i], "-?") == 0)
		{
			printf("Usage: \n");
			printf("mydir --help or -? or /?  - Prints this help screen");
			printf("mydir -L or -l            - Prints this help screen");
			return 0;
		}
		// Check if we need to list out everything
		else if (stricmp(argv[i], "-l") == 0 ||
			stricmp(argv[i], "-L") == 0)
		{
			longName = TRUE;
		}
		else
		{
			strcpy_s(path, MAX_PATH, argv[i]);
		}
	}

	return list_directories(path, longName);
}