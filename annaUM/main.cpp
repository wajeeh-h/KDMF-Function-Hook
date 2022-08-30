#include <stdio.h>
#include <libloaderapi.h>
#include "hack.h"

int main()
{
	printf("[annaKMDF]: Loading...\n");
	LoadLibraryA("user32,dll");
	LoadLibraryA("win32u.dll");
	Hack::init();
	return 0;
}