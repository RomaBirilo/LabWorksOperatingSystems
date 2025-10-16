#include "min_max.h"
#include <windows.h>
#include <iostream>
#include <vector>
using namespace std;

vector<int>  min_max(vector <int>& array)
{
	vector<int> res;
	if (array.size() == 0)
	{
		cout << "This array is empty" << endl;
		res.push_back(-1);
		res.push_back(-1);
		return res;
	}
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
	res.push_back(min);
	res.push_back(max);
	return res;
}

DWORD WINAPI MinMaxWrapper(LPVOID lpParam)
{
	ThreadMinMax* data = reinterpret_cast<ThreadMinMax*>(lpParam);
	if (data && data->array) 
{
		data->result = min_max(*data->array);
	}
	return 0;
}
