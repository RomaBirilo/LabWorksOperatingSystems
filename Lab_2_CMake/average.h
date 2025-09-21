#pragma once
#include <windows.h>
#include <iostream>
#include <vector>
using namespace std;

void average(vector <int>& array)
{
	int sum = 0;
	for (size_t i = 0; i < array.size(); i++)
	{
		sum += array[i];
		Sleep(12);
	}

	cout << "Average:" << sum/array.size() << endl;
}

DWORD WINAPI AverageWrapper(LPVOID lpParam)
{
	auto vec = reinterpret_cast<std::vector<int>*>(lpParam);
	if (vec && !vec->empty()) {
		try { average(*vec); }
		catch (...) { return 1; }
	}
	return 0;
}
