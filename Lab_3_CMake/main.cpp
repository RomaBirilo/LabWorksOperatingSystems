#include <windows.h>
#include <iostream>
#include <vector>
#include "ThreadData.h"
#include "marker.h"
#include <set>
#include <stdexcept>
using namespace std;
int main()
{
	cout << "Input size of array:";
	int n;
	cin >> n;
	vector<int> array(n, 0);
	cout << "Input number of marker threads(<=64):";
	int threads_num;
	cin>> threads_num;

	CRITICAL_SECTION cs;
	InitializeCriticalSection(&cs);
	vector<ThreadData*> threadData(threads_num,nullptr);
	HANDLE* hThreads = new HANDLE[threads_num];
	vector <HANDLE> hStopEvents;
	vector <int> indices;
	HANDLE hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	
	for (int i = 0; i < threads_num; i++)
	{
		HANDLE hTerminateEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		HANDLE hContinueEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		HANDLE hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		HANDLE hCompletedEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		
		if (!hTerminateEvent || !hStopEvent || !hCompletedEvent) 
		{
			cout << "CreateEvent failed\n";
			return 1;
		}

		ThreadData* data = new ThreadData{ &array, i + 1, &cs,hStartEvent,hStopEvent,hTerminateEvent,hContinueEvent,hCompletedEvent};
		threadData[i] = data;
		hStopEvents.push_back(hStopEvent);
		indices.push_back(i);

		
		try
		{
			DWORD ThreadID;
			HANDLE h = CreateThread(NULL, 0, marker, data, 0, &ThreadID);
			if (!h)
			{
				cout << "CreateThread failed for thread " << i << " Error: " << GetLastError() << "\n";
			}
			else
			{
				hThreads[i] = h;
			}
		}
		catch (const invalid_argument& e)
		{
			cout << e.what();
			return 0;
		}
		
	}

	SetEvent(hStartEvent);

	int complete_number=0;
	set <int> completed;

	while (!indices.empty())
	{
		int curCount = hStopEvents.size();

		if (WaitForMultipleObjects(curCount, hStopEvents.data(), TRUE, INFINITE) != WAIT_OBJECT_0)
		{
			cout << "WaitForMultipleObjects failed: " << GetLastError() << "\n";
			break;
		}

		cout << "Array before:";
		for (int i = 0; i < array.size(); i++)
		{
			cout << array[i]<<" ";
		}
		cout << endl;

		cout << "Input number of thread that you want to complete:";
		cin >> complete_number;
		if (complete_number < 1 || complete_number > threads_num) 
		{
			cout << "Invalid thread number\n";
			continue;
		}
		if (completed.find(complete_number) == completed.end())
			completed.insert(complete_number);
		else
		{
			cout << "You must enter the number of a stream that has not yet completed.";
			continue;
		}
			

		int idx = complete_number - 1;

		SetEvent(threadData[idx]->hTerminateEvent);
		for (int i : indices) 
		{
			if (i == idx) 
				continue;
			SetEvent(threadData[i]->hContinueEvent);
		}
		WaitForSingleObject(threadData[idx]->hCompletedEvent, INFINITE);
		WaitForSingleObject(hThreads[idx], INFINITE);

		cout << "Array after "<<complete_number<<" thread completing:";
		for (int i = 0; i < array.size(); i++)
		{
			cout << array[i] << " ";
		}
		cout << endl << endl;

		vector<HANDLE> newStopEvents;
		vector<int> newIndices;
		for (int i = 0; i < indices.size(); i++) 
		{
			int globalIdx = indices[i];
			if (globalIdx == idx) 
			{
	
				CloseHandle(hThreads[globalIdx]); hThreads[globalIdx] = NULL;
				CloseHandle(threadData[globalIdx]->hStopEvent);
				CloseHandle(threadData[globalIdx]->hTerminateEvent);
				CloseHandle(threadData[globalIdx]->hContinueEvent);
				CloseHandle(threadData[globalIdx]->hCompletedEvent);

				delete threadData[globalIdx];
				threadData[globalIdx] = nullptr;
			}
			else 
			{
				ResetEvent(threadData[globalIdx]->hStopEvent);
				newStopEvents.push_back(threadData[globalIdx]->hStopEvent);
				newIndices.push_back(globalIdx);
			}
		}
		hStopEvents.swap(newStopEvents);
		indices.swap(newIndices);
		
	}
	
	for (int i = 0; i < threads_num; i++)
	{
		if (hThreads[i])
		{
			WaitForSingleObject(hThreads[i], INFINITE);
			CloseHandle(hThreads[i]);
		}
		if (threadData[i]) 
		{
			CloseHandle(threadData[i]->hTerminateEvent);
			CloseHandle(threadData[i]->hContinueEvent);
			CloseHandle(threadData[i]->hStopEvent);
			CloseHandle(threadData[i]->hCompletedEvent);
			delete threadData[i];
		}
	}
	CloseHandle(hStartEvent);
	DeleteCriticalSection(&cs);

	cout << "Main finished.\n";
}