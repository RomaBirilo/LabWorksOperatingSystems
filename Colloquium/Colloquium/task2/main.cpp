
#include "main.h"
#include <vector>

using namespace std;

int main()
{
	cout << "Input size of your array:";
	int size;
	cin >> size;
	vector <int> array_with_duplicates(size,0);
	cout << "Input array:";
	for (int i = 0; i < size; i++)
	{
		cin>>array_with_duplicates[i];
	}
	
	vector<int> array_without_duplicates;
	array_without_duplicates = RemoveRepetitions(array_with_duplicates);

	cout << "Result:";
	for (int num : array_without_duplicates)
	{
		cout << num<<" ";
	}
}
