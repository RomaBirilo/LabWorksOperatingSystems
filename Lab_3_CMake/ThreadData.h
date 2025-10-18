#pragma once
#include <windows.h>
#include <vector>
struct ThreadData
{
	std::vector<int>* array;
	int number;
	CRITICAL_SECTION* cs;
	HANDLE hStartEvent;
	HANDLE hStopEvent;
	HANDLE hTerminateEvent;
	HANDLE hContinueEvent;
	HANDLE hCompletedEvent;

};

