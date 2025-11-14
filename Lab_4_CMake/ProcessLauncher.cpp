#include "ProcessLauncher.h"
#include <windows.h>

bool ProcessLauncher(wstring& cmdLine, PROCESS_INFORMATION &pi)
{
    STARTUPINFOW si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    if (CreateProcessW(
        NULL,
        &cmdLine[0],
        NULL,
        NULL,
        FALSE,
        CREATE_NEW_CONSOLE,
        NULL,
        NULL,
        &si,
        &pi
    ))
        return true;
    else
        return false;
}
