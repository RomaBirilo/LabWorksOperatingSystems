#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <stdexcept>
using std::vector;
using std::runtime_error;
using std::to_string;

struct employee
{
	int num; 
	char name[10]; 
	double hours;
};

vector<PROCESS_INFORMATION>& LaunchProcesses(int processesCount);
void CloseProcesses(vector<PROCESS_INFORMATION>& processes);