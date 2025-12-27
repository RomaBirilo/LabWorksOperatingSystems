#include "Server.h"
using namespace std;

int main()
{
	cout << "Input name of binary file:";
	string fileName;
	getline(cin, fileName);
	cout << "Input number of employees:";
	int employeesNumber;
	cin >> employeesNumber;
	WriteFile(fileName, employeesNumber);
	cout << "Your file:" << endl;
	ShowFile(fileName, employeesNumber);

	cout << "Input count of processec Client:";
	int processesCount;
	cin >> processesCount;

	vector<EmployeeLock> locks(employeesNumber);
	InitLocks(employeesNumber, locks);
	ThreadParams* params = new ThreadParams{&locks, &fileName, employeesNumber};
	vector <HANDLE> threads;

	try 
	{
		threads = LaunchThreads(processesCount, params);
	}
	catch (const runtime_error& ex)
	{
		cout << ex.what();
	}

	WaitForMultipleObjects(processesCount, threads.data(), TRUE, INFINITE);
	CloseLocks(locks);
	CloseThreads(threads);
	cout << "Your file:" << endl;
	ShowFile(fileName, employeesNumber);
	cout << "Input any key to finish programm:";   
	char c;
	cin >> c;
	return 0;
}
