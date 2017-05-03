// Kainoa Seto
// MyDir OS program that replicates the basic functionality of the MSDOS DIR
// 5/4/17
// main.c

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
		if (strcmp(argv[i], "/?") == 0 ||
			strcmp(argv[i], "--help") == 0 ||
			strcmp(argv[i], "-?") == 0)
		{
			printf("Usage: \n");
			printf("MyDir [pattern]           - Prints all files or dirs that match optional pattern\n");
			printf("MyDir --help or -? or /?  - Prints this help screen\n");
			printf("MyDir -L or -l [pattern]  - Prints any dirs or files that match optional pattern with attributes\n");
			return 0;
		}
		// Check if we need to list out everything
		else if (strcmp(argv[i], "-l") == 0 ||
			strcmp(argv[i], "-L") == 0)
		{
			longName = TRUE;
		}
		else
		{
			strcpy_s(path, MAX_PATH, argv[i]);
		}
	}

	return list__dir(path, longName);
}