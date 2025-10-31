#include <gtest/gtest.h>
#include <windows.h>
#include <vector>
#include "marker.h"
#include "ThreadData.h"
using std::vector;
using std::invalid_argument;
using std::set;

TEST(TestMarker, WorkCheckNullPtr)
{
	EXPECT_THROW(marker(nullptr), invalid_argument);
}

TEST(TestMarker, WorkCheckEmptyArray)
{
	vector<int> array;
	HANDLE hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	HANDLE hTerminateEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE hContinueEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	HANDLE hCompletedEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	CRITICAL_SECTION cs;
	DWORD ThreadID;
	ThreadData* data = new ThreadData{ &array, 1, &cs,hStartEvent,hStopEvent,hTerminateEvent,hContinueEvent,hCompletedEvent };
	EXPECT_THROW(marker(data), invalid_argument);
	CloseHandle(hStartEvent);
	CloseHandle(hTerminateEvent);
	CloseHandle(hContinueEvent);
	CloseHandle(hStopEvent);
	CloseHandle(hCompletedEvent);
}

TEST(TestMarker, WorkCheckMarkeredArray)
{
	vector<int> array(4, 0);
	vector<int> result_array{1,1,1,1};

	CRITICAL_SECTION cs;
	InitializeCriticalSection(&cs);

	HANDLE hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	HANDLE hTerminateEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE hContinueEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	HANDLE hCompletedEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	ASSERT_NE(hStartEvent, (HANDLE)NULL);
	ASSERT_NE(hTerminateEvent, (HANDLE)NULL);
	ASSERT_NE(hContinueEvent, (HANDLE)NULL);
	ASSERT_NE(hStopEvent, (HANDLE)NULL);
	ASSERT_NE(hCompletedEvent, (HANDLE)NULL);

	DWORD ThreadID;
	ThreadData* data = new ThreadData{ &array, 1, &cs,hStartEvent,hStopEvent,hTerminateEvent,hContinueEvent,hCompletedEvent };
	HANDLE h = CreateThread(NULL, 0, marker, data, 0, &ThreadID);
	ASSERT_NE(h, (HANDLE)NULL);

	SetEvent(hStartEvent);
	ASSERT_EQ(WaitForSingleObject(hStopEvent, 2000), WAIT_OBJECT_0);

	EXPECT_EQ(array, result_array);

	SetEvent(hTerminateEvent);

	ASSERT_EQ(WaitForSingleObject(hStopEvent, 2000), WAIT_OBJECT_0);

	ASSERT_EQ(WaitForSingleObject(h, 2000), WAIT_OBJECT_0);

	CloseHandle(h);
	CloseHandle(hStartEvent);
	CloseHandle(hTerminateEvent);
	CloseHandle(hContinueEvent);
	CloseHandle(hStopEvent);
	CloseHandle(hCompletedEvent);

	DeleteCriticalSection(&cs);
	delete data;
}


TEST(TestMarker, WorkCheckTerminate)
{
	vector<int> array(4,0);
	vector<int> result_array{ 0,0,0,0 };

	CRITICAL_SECTION cs;
	InitializeCriticalSection(&cs);

	HANDLE hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	HANDLE hTerminateEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE hContinueEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	HANDLE hCompletedEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	
	ASSERT_NE(hStartEvent, (HANDLE)NULL);
	ASSERT_NE(hTerminateEvent, (HANDLE)NULL);
	ASSERT_NE(hContinueEvent, (HANDLE)NULL);
	ASSERT_NE(hStopEvent, (HANDLE)NULL);
	ASSERT_NE(hCompletedEvent, (HANDLE)NULL);

	DWORD ThreadID;
	ThreadData* data = new ThreadData{ &array, 1, &cs,hStartEvent,hStopEvent,hTerminateEvent,hContinueEvent,hCompletedEvent };
	HANDLE h = CreateThread(NULL, 0, marker, data, 0, &ThreadID);
	ASSERT_NE(h, (HANDLE)NULL);

	SetEvent(hStartEvent);
	ASSERT_EQ(WaitForSingleObject(hStopEvent, 2000), WAIT_OBJECT_0);

	SetEvent(hTerminateEvent);

	ASSERT_EQ(WaitForSingleObject(hStopEvent, 2000), WAIT_OBJECT_0);

	ASSERT_EQ(WaitForSingleObject(h, 2000), WAIT_OBJECT_0);


	EXPECT_EQ(array, result_array);

	CloseHandle(h);
	CloseHandle(hStartEvent);
	CloseHandle(hTerminateEvent);
	CloseHandle(hContinueEvent);
	CloseHandle(hStopEvent);
	CloseHandle(hCompletedEvent);

	DeleteCriticalSection(&cs);
	delete data;
}

TEST(TestMarker, WorkCheckCriticalSections)
{
	const int size = 3;
	const int threads_num = 3;
	vector<int> array(size, 0);

	CRITICAL_SECTION cs;
	InitializeCriticalSection(&cs);
	vector<ThreadData*> threadData(threads_num, nullptr);
	HANDLE* hThreads = new HANDLE[threads_num];
	vector <HANDLE> hStopEvents;
	HANDLE hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	ASSERT_NE(hStartEvent, (HANDLE)NULL);

	for (int i = 0; i < threads_num; i++)
	{
		HANDLE hTerminateEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		HANDLE hContinueEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		HANDLE hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		HANDLE hCompletedEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

		ASSERT_NE(hTerminateEvent, (HANDLE)NULL);
		ASSERT_NE(hContinueEvent, (HANDLE)NULL);
		ASSERT_NE(hStopEvent, (HANDLE)NULL);
		ASSERT_NE(hCompletedEvent, (HANDLE)NULL);

		ThreadData* data = new ThreadData{ &array, i + 1, &cs,hStartEvent,hStopEvent,hTerminateEvent,hContinueEvent,hCompletedEvent };
		threadData[i] = data;
		hStopEvents.push_back(hStopEvent);

		DWORD ThreadID;
		HANDLE h = CreateThread(NULL, 0, marker, data, 0, &ThreadID);
		ASSERT_NE(h, (HANDLE)NULL);
		hThreads[i] = h;
	}

	SetEvent(hStartEvent);
	ASSERT_EQ(WaitForMultipleObjects(threads_num, hStopEvents.data(), TRUE, 5000), WAIT_OBJECT_0);

	for (int i = 0; i < threads_num; i++)
	{
		SetEvent(threadData[i]->hTerminateEvent);
		ASSERT_EQ(WaitForSingleObject(threadData[i]->hCompletedEvent, 2000), WAIT_OBJECT_0);
		for (int j = 0; j < array.size(); j++)
		{
			EXPECT_NE(array[j], i + 1);
		}
	}

	for (int i = 0; i < threads_num; i++)
	{
		ASSERT_EQ(WaitForSingleObject(hThreads[i], 2000), WAIT_OBJECT_0);
		CloseHandle(hThreads[i]);
		CloseHandle(threadData[i]->hTerminateEvent);
		CloseHandle(threadData[i]->hContinueEvent);
		CloseHandle(threadData[i]->hStopEvent);
		CloseHandle(threadData[i]->hCompletedEvent);
		delete threadData[i];
	}
	CloseHandle(hStartEvent);
	DeleteCriticalSection(&cs);
}