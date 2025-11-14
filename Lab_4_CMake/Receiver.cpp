
#include "Receiver.h"
#include "ProcessLauncher.h"
#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int main()
{
	cout << "Input name of your binary file:";
	wstring file_name;
	wcin >> file_name;
	cout << "Input count of notes in your binary file:";
	int notes_count;
	cin >> notes_count;
	
	cout << "Input count of processes Sender:";
	int sender_count;
	cin >> sender_count;
	wstring comand = L"\"Sender.exe\" \"" + file_name + L"\"";
	vector <PROCESS_INFORMATION> processes(sender_count);
	vector <HANDLE> events(sender_count);

	for (int i = 0; i < sender_count; i++)
	{
		PROCESS_INFORMATION pi;
		wstring event_name = to_wstring(i + 1);
		comand += event_name;
		if (!ProcessLauncher(comand, pi))
		{
			cout << "Cannot create process number "<< i+1;
			return 0;
		}
		
		HANDLE hStartEvent = CreateEventW(NULL, FALSE, FALSE, event_name.c_str());
		events[i] = hStartEvent;
		processes[i] = pi;
	}
	WaitForMultipleObjects(sender_count, events.data(), TRUE, INFINITE);


	ifstream fin(file_name, ios::binary);
	if (!fin)
	{
		cout << "Cannot create binary file";
		return 0;
	}

	bool work = true;
	while (work)
	{
		cout << "Choose:" << endl;
		cout << "1.Read note from binary file;" << endl;
		cout << "2.Finish work";
		int n;
		cin >> n;
		if (n != 1 && n != 2)
			continue;
		if (n == 1)
		{
			string note;
			fin >> note;
			cout << "Note from binary file:" << note << endl;
		}
		else if (n == 2)
			work = false;
	}

}
