#include "Server.h"
using namespace std;

int main()
{
	cout << "Input name of binary file:";
	string fileName;
	getline(cin, fileName);
	ofstream fout(fileName, ios::binary);

	cout << "Input number of students";
	int employeesNumber;
	cin >> employeesNumber;
	for (size_t i = 0; i < employeesNumber; i++)
	{
		employee emp;

		cout << "Input information for employee " << i + 1 << ":" << endl;
		cout << "Employee's id:";
		cin >> emp.num;
		cout << "Employee's name:";
		cin >> emp.name;
		cout << "Work hours:";
		cin >> emp.hours;

		fout.write(reinterpret_cast<char*>(&emp), sizeof(emp));
	}
	fout.close();

	cout << "Input count of processec Client:";
	int processesCount;
	cin >> processesCount;
	vector <PROCESS_INFORMATION> processes;

	try 
	{
		processes = LaunchProcesses(processesCount);
	}
	catch (const runtime_error& ex)
	{
		cout << ex.what();
	}






	CloseProcesses(processes);
}
