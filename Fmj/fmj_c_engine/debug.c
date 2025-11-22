#include "framework.h"
#include "debug.h"

void init_console()
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	printf("add console success!\n");
}

void release_console()
{
	FreeConsole();
}