#pragma once
#include "min_max.h"
#include <windows.h>
#include <iostream>
#include <vector>
using namespace std;

void min_max(vector <int>& array)
{
	int max = array[0], min = array[0];
	for (size_t i = 0; i < array.size(); i++)
	{
		if (array[i] > max)
		{
			max = array[i];
			Sleep(7);
		}
		if (array[i] < min)
		{
			min = array[i];
			Sleep(7);
		}

	}

	cout << "Min element:" << min << endl;
	cout << "Max element:" << max << endl;
}

DWORD WINAPI MinMaxWrapper(LPVOID lpParam)
{
	auto vec = reinterpret_cast<std::vector<int>*>(lpParam);
	if (vec && !vec->empty()) {
		try { min_max(*vec); }
		catch (...) { return 1; }
	}
	return 0;
}
