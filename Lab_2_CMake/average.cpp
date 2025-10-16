#include "average.h"
#include <windows.h>
#include <iostream>
#include <vector>
using namespace std;

double average(vector <int>& array)
{
	if (array.size() == 0)
	{
		cout << "This array is empty" << endl;
		return -1.0;
	}
	double sum = 0;
	for (size_t i = 0; i < array.size(); i++)
	{
		sum += array[i];
		Sleep(12);
	}

	return sum / array.size();
}

DWORD WINAPI AverageWrapper(LPVOID lpParam)
{
	ThreadAverage* data = reinterpret_cast<ThreadAverage*>(lpParam);
	if (data && data->array)
	{
		data->result = average(*data->array);
	}
	return 0;
}
