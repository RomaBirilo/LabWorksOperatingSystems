#pragma once
#include <string>
#include <windows.h>
using std::wstring;

bool ProcessLauncher(wstring & cmdLine, PROCESS_INFORMATION& pi);
