
#include "Receiver.h"
#include "ProcessLauncher.h"
#include "BufferClass.h"
#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include <vector>
#include <optional>
using namespace std;

int main()
{
	const int note_size = 20;
	const int header = sizeof(int) * 2;

	cout << "Input name of your binary file:";
	string file_name;
	getline(cin, file_name);
	if (file_name.empty()) 
	{
		cerr << "Empty file name\n";
		return 1;
	}
	
	cout << "Input count of notes in your binary file:";
	int notes_count;
	cin >> notes_count;
	BufferClass buffer(file_name, notes_count);

	if(!buffer.initialize())
	{
		cerr << "Cannot create file\n";
		return 1;
	}
	cin.ignore(INT_MAX, '\n');

	cout << "Input count of processes Sender:";
	int sender_count;
	cin >> sender_count;

	string mutexName = file_name + "_mutex";
	string emptySemName = file_name + "_empty";
	string fullSemName = file_name + "_full";

	HANDLE hMutex = CreateMutexA(NULL, FALSE, mutexName.c_str());
	if (!hMutex) 
	{ 
		cout << "CreateMutex failed\n"; 
		return 1; 
	}

	HANDLE hEmpty = CreateSemaphoreA(NULL, notes_count, notes_count, emptySemName.c_str());
	if (!hEmpty) 
	{ 
		cout << "CreateSemaphore(empty) failed\n"; 
		return 1; 
	}

	HANDLE hFull = CreateSemaphoreA(NULL, 0, notes_count, fullSemName.c_str());
	if (!hFull) 
	{ 
		cout << "CreateSemaphore(full) failed\n"; 
		return 1; 
	}

	vector <PROCESS_INFORMATION> processes(sender_count);
	vector <HANDLE> events(sender_count, NULL);
	
	for (int i = 0; i < sender_count; i++)
	{
		PROCESS_INFORMATION pi;
		string eventName = file_name + "_ready_" + to_string(i + 1);
		HANDLE hReadyEvent = CreateEventA(NULL, FALSE, FALSE, eventName.c_str());
		if (!hReadyEvent) 
		{ 
			cout << "CreateEvent failed for " << eventName << "\n"; 
			return 1;
		}
		events[i] = hReadyEvent;

		string comand = "Sender.exe \"" + file_name + "\" \"" + eventName + "\" \"" +
			mutexName + "\" \"" + emptySemName + "\" \"" + fullSemName + "\" \"" + to_string(notes_count)+"\"";
		vector<char> cmdbuf(comand.begin(), comand.end());
		cmdbuf.push_back('\0');
		if (!ProcessLauncher(cmdbuf, pi))
		{
			cout << "Cannot create process number "<< i+1;
			return 0;
		}
		processes[i] = pi;
	}

	if (WaitForMultipleObjects(sender_count, events.data(), TRUE, INFINITE) == WAIT_FAILED)
	{
		cout << "WaitForMultipleObjects failed, error = " << GetLastError() << "\n";
		return 1;
	}

	bool work = true;
	while (work)
	{
		cout << "Choose:" << endl;
		cout << "1.Read note from binary file;" << endl;
		cout << "2.Finish work" << endl;
		int n;
		cin >> n;
		if (n != 1 && n != 2)
			continue;
		if (n == 1)
		{
			WaitForSingleObject(hFull, INFINITE);
			WaitForSingleObject(hMutex, INFINITE);
			optional<string> note = buffer.readNote();
			if (!note.has_value())
				cout << "Cannot read note from file" << endl;
			cout << "Note from binary file:" << note.value() << endl;
			ReleaseMutex(hMutex);
			ReleaseSemaphore(hEmpty, 1, NULL);
			
		}
		else if (n == 2)
			work = false;
	}

	for (int i = 0; i < sender_count; i++) 
	{
		WaitForSingleObject(processes[i].hProcess, 1000);
		CloseHandle(processes[i].hProcess);
		CloseHandle(processes[i].hThread);
		CloseHandle(events[i]);
	}

	CloseHandle(hMutex);
	CloseHandle(hEmpty);
	CloseHandle(hFull);

	cout << "Receiver finished\n";
	return 0;
	
}
