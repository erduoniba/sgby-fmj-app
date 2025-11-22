#pragma once
#include <stdio.h>

#ifdef _DEBUG
#define INIT_CONSOLE	init_console
#define RELEASE_CONSOLE	release_console
#define LOG				printf
//#define LOG(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define INIT_CONSOLE()
#define RELEASE_CONSOLE()
#define LOG(...)
#endif // _DEBUG

void init_console();
void release_console();
