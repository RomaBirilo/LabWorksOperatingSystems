

#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;
void CountFactorials(vector <long long> &factorials, int n)
{
	if (n < 0)
		throw invalid_argument("n can't be negative");
	factorials[0] = 1;
	for (int i = 1; i < n+1; i++)
	{
		factorials[i] = factorials[i - 1] * i;
	}
}



