#pragma once
#include <vector>
#include <windows.h>
using std::vector;
bool ProcessLauncher(vector<char>& cmdLine, PROCESS_INFORMATION& pi);
