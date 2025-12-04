#pragma once
#include <vector>
#include <string>
#include <windows.h>
using std::vector;
using std::string;

bool processLauncher(vector<char>& cmdLine, PROCESS_INFORMATION& pi);
HANDLE createReadyEvent(const string& baseName, int index);
bool launchSenderProcess(const string& command, PROCESS_INFORMATION& pi);