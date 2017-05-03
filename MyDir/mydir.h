// Kainoa Seto
// MyDir OS program that replicates the basic functionality of the MSDOS DIR
// Main header file containing all includes and function prototypes
// 5/4/17
// mydir.h

#ifndef __MYDIR_H__
#define __MYDIR_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <WinBase.h>

// Checks to see if the path contains a directory or a partial directory name via wildcards
// char* pattern: A pattern or path that leads to a suspected directory
// Returns a bool if a directory exists in the pattern or false if not
BOOL directory_exists(char* pattern);

// Checks to see if the path contains a file or a partial filename via wildcards
// char* pattern: A pattern or path that leads to a suspected file
// Returns a bool if a file exists in the pattern or false if not
BOOL file_exists(char* pattern);

// Lists out directories and files vertically with the option of all their attributes
// char*: path or pattern to files/directories
// BOOL: TRUE if the file attributes should be printed out
// returns 0 if successful or 1 if it cannot find anything
int list__dir(char* path, BOOL longName);

#endif __MYDIR_H__