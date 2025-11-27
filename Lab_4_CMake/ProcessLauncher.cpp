#include "ProcessLauncher.h"
#include <vector>
#include <windows.h>
using std::vector;

bool ProcessLauncher(vector<char>& cmdLine, PROCESS_INFORMATION &pi)
{
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    if (CreateProcessA(
        NULL,
        cmdLine.data(),
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
