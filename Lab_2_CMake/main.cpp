
#include "main.h"
#include "min_max.h"
#include "average.h"
#include <iostream>
#include <vector>

using namespace std;

int main()
{
	cout << "Input number of elements in array:";
	int n;
	cin >> n;

	cout << "Input your array:";
	vector <int> array(n);
	for (size_t i = 0; i < n; i++)
	{
		cin>>array[i];
	}



	DWORD IDMinMax;
	HANDLE hMinMax = CreateThread(NULL, 0, MinMaxWrapper, &array, 0, &IDMinMax);
	

	DWORD IDAverage;
	HANDLE hAverage = CreateThread(NULL, 0, AverageWrapper, &array, 0, &IDAverage);

	WaitForSingleObject(hAverage, INFINITE);
	WaitForSingleObject(hMinMax, INFINITE);
	CloseHandle(hMinMax);
	CloseHandle(hAverage);
}
