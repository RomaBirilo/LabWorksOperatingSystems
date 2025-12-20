#include "resources.h"

vector<PROCESS_INFORMATION>& LaunchProcesses(int processesCount)
{
	vector<PROCESS_INFORMATION> processes(processesCount);
	char command[24] = "Client.exe";
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	for (size_t i = 0; i < processesCount; i++)
	{
		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(pi));
		if (!CreateProcessA(
			NULL,
			command,
			NULL,
			NULL,
			FALSE,
			CREATE_NEW_CONSOLE,
			NULL,
			NULL,
			&si,
			&pi))
		{
			DWORD err = GetLastError();
			throw runtime_error("Failed to create process. Win32 error code: " + to_string(err));
		}
		processes[i] = pi;
	}
	return processes;
}

void CloseProcesses(vector<PROCESS_INFORMATION>& processes)
{
	for (int i = 0; i < processes.size(); i++)
	{
		WaitForSingleObject(processes[i].hProcess, 1000);
		CloseHandle(processes[i].hProcess);
		CloseHandle(processes[i].hThread);
	}
}
