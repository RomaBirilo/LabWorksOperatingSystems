

#include "Kollok-1.h"
#include <vector>
using namespace std;

int main()
{
	
		cout << "Input n:";
		int n;
		cin >> n;
		vector<long long> factorials(n + 1, 0);
		factorials[0] = 1;
		for (int i = 1; i < n + 1; i++)
		{
			factorials[i] = factorials[i - 1] * i;
		}
		cout << "First n factorials:";
		for (int i = 1; i < factorials.size(); i++)
		{
			cout << factorials[i] << " ";
		}
		return 0;
	}
	
	
}
