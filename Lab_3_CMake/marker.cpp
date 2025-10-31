#include "marker.h"
#include "ThreadData.h"
#include <iostream>
#include <stdexcept>
using std::cout;
using std::invalid_argument;

DWORD WINAPI marker(LPVOID lpParam)
{
	ThreadData* data = reinterpret_cast<ThreadData*>(lpParam);
	if (!data || !data->array || !data->cs || !data->hStartEvent ||
		!data->hStopEvent || !data->hTerminateEvent || !data->hContinueEvent || !data->hCompletedEvent)
	{
		throw invalid_argument("ThreadData pointer invalid");
	}
	if (data->array->empty())
		throw invalid_argument("Array is empty");
	
	WaitForSingleObject(data->hStartEvent, INFINITE);

	srand(data->number);
	int random_number=0, unmarkered_index=-1;
	int sleep_time = 5;
	int count = 0;
	bool next_iteration = true;

	while (true)
	{
		
		random_number = rand() % data->array->size();
		Sleep(sleep_time);

		EnterCriticalSection(data->cs);
		if ((*data->array)[random_number] == 0)
		{
			(*data->array)[random_number] = data->number;
			count++;
			LeaveCriticalSection(data->cs);

			Sleep(sleep_time);
			continue;
		}
		else
		{
			unmarkered_index = random_number;
			LeaveCriticalSection(data->cs);

			cout << "It is marker with " << data->number << " number\n";
			cout << "Count of markered elements:" << count << "\n";
			cout << "Index of the element, that we can't mark:" << unmarkered_index << "\n\n";

			SetEvent(data->hStopEvent);

			HANDLE waits[2] = { data->hContinueEvent, data->hTerminateEvent };
			DWORD w = WaitForMultipleObjects(2, waits, FALSE, INFINITE);

			if (w == WAIT_OBJECT_0)
				continue;
			else if (w == WAIT_OBJECT_0 + 1)
			{
				EnterCriticalSection(data->cs);
				for (int i = 0; i < data->array->size(); i++)
				{
					if ((*data->array)[i] == data->number)
						(*data->array)[i] = 0;
				}
				LeaveCriticalSection(data->cs);

				SetEvent(data->hCompletedEvent);
				return 0;
			}
			else
				return 1;

		}		
	}
}

