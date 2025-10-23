

#include "main.h"
#include <vector>
#include <stdexcept>
using namespace std;

int main()
{
	try
	{
		cout << "Input n:";
		int n;
		cin >> n;
		if (!cin) 
			throw runtime_error("Input error");
		if (n < 0)
			throw invalid_argument("n can't be negative");

		vector<long long> factorials(n+1, 0);

		CountFactorials(factorials, n);

		cout << "First n factorials:";
		for (int i = 1; i < factorials.size(); i++)
		{
			cout << factorials[i] << " ";
		}
		return 0;
	}
	catch (const invalid_argument& er)
	{
		cout << er.what();
	}
	catch (const runtime_error& er)
	{
		cout << er.what();
	}
	catch (const exception& e)
    {
		cout << "Unknown error";
    }
}
