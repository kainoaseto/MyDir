#ifndef __MYDIR_H__
#define __MYDIR_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <WinBase.h>

BOOL directory_exists(char* path);
BOOL file_exists(char* path);
int list_directories(char* path, BOOL longName);

#endif __MYDIR_H__