

#pragma once

#include <iostream>
#include <vector>
#include <set>
using namespace std;

vector <int> RemoveRepetitions(vector <int> & array)
{
	vector <int> result;
	set <int> duplicates;
	for (int num : array)
	{
		if (duplicates.find(num) == duplicates.end())
		{
			result.push_back(num);
			duplicates.insert(num);
		}
	}
	return result;
}
