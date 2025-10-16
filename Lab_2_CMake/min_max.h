#pragma once
#include <windows.h>
#include <vector>

struct ThreadMinMax 
{
    std::vector<int>* array;
    std::vector<int> result;
};

DWORD WINAPI MinMaxWrapper(LPVOID lpParam);
std::vector<int> min_max(std::vector<int>& array);
