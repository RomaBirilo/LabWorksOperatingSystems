
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

	ThreadMinMax MinMaxData{ &array, {} };
	ThreadAverage AverageData{ &array, 0.0 };

	DWORD IDMinMax;
	HANDLE hMinMax = CreateThread(NULL, 0, MinMaxWrapper, &MinMaxData, 0, &IDMinMax);
	
	DWORD IDAverage;
	HANDLE hAverage = CreateThread(NULL, 0, AverageWrapper, &AverageData, 0, &IDAverage);


	if (!hMinMax || !hAverage)
	{
		cerr << "Failed to create threads!" << endl;
		return 1;
	}

	WaitForSingleObject(hAverage, INFINITE);
	WaitForSingleObject(hMinMax, INFINITE);
	CloseHandle(hMinMax);
	CloseHandle(hAverage);

	for (int i = 0; i < array.size(); i++)
	{
		if (array[i] == MinMaxData.result[0] || array[i] == MinMaxData.result[1])
			array[i] = static_cast<int>(AverageData.result);
	}
	cout << "Min element:" << MinMaxData.result[0] << endl;
	cout << "Max element:" << MinMaxData.result[1] << endl;
	cout << "Average:" << AverageData.result << endl;
	cout << "New array:";
	for (int i = 0; i < array.size(); i++)
	{
		cout << array[i] << " ";
	}

}
