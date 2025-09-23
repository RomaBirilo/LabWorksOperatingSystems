#pragma once
#include <windows.h>
#include <iostream>
#include <vector>
using namespace std;

double average(vector <int>& array)
{
	if (array.size()==0)
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

	cout << "Average:" << sum/array.size() << endl;
	return sum / array.size();
}

DWORD WINAPI AverageWrapper(LPVOID lpParam)
{
	auto vec = reinterpret_cast<std::vector<int>*>(lpParam);
	if (vec && !vec->empty()) 
	{
		try 
		{ 
			average(*vec); 
		}
		catch (...) 
		{ 
			return 1; 
		}
	}
	return 0;
}
