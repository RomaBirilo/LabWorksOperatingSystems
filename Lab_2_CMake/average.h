#pragma once
#include <windows.h>
#include <vector>

struct ThreadAverage
{
   std::vector<int>* array;
   double result;
};

DWORD WINAPI AverageWrapper(LPVOID lpParam);
double average(std::vector <int>& array);
