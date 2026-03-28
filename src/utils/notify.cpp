#include "notify.h"

void message_handler(const char* title, const char* message, char* flag) {

    #ifdef _WIN32
        UINT flags;
        const char* msg;
        if (!strcmp(flag, "-i")) {
            flags = MB_OK | MB_ICONINFORMATION;
        }
        else if (!strcmp(flag, "-w")) {
            flags = MB_OK | MB_ICONWARNING;
        }
        else if (!strcmp(flag , "-e")) {
            flags = MB_OK | MB_ICONERROR;
        }
        else if (!strcmp(flag, "-q")) {
            flags = MB_OK | MB_ICONQUESTION;
        }
        MessageBoxA(NULL, message, title, flags);
    #else
        printf("[%s] %s\n", title, message);
    #endif
}