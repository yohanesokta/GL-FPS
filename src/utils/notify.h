#pragma once
#ifdef _WIN32
#include <windows.h>
#else
#include <cstdio>
#include <unistd.h>
#endif

void message_handler(const char *title, const char *message, const char *flag);