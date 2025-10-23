
#include "main.h"
#include <vector>
#include <stdexcept>
using namespace std;

int main()
{
	try
	{
		cout << "Input size of your array:";
		int size;
		cin >> size;
		if (size < 0)
		{
			throw invalid_argument("Array size must be positive.");
		}
		vector <int> array_with_duplicates(size, 0);
		cout << "Input array:";
		for (int i = 0; i < size; i++)
		{
			cin >> array_with_duplicates[i];
			if (cin.fail()) {
				throw invalid_argument("Invalid input. Please enter integers only.");
			}
		}

		vector<int> array_without_duplicates;
		array_without_duplicates = RemoveRepetitions(array_with_duplicates);

		cout << "Result:";
		for (int num : array_without_duplicates)
		{
			cout << num << " ";
		}
	}
	catch (const invalid_argument& er)
	{
		cout << er.what();
	}

}
