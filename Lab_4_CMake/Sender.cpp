#include "Sender.h"
#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int main(int argc, char* argv[])
{
	const int note_size = 20;

	if (argc < 6) {
		cerr << "Usage: Sender.exe <filename> <readyEventName> <mutexName> <emptySemName> <fullSemName>\n";
		return 1;
	}

	string file_name = argv[1];
	string eventName = argv[2];
	string mutexName = argv[3];
	string emptySemName = argv[4];
	string fullSemName = argv[5];

	HANDLE hReadyEvent = OpenEventA(EVENT_MODIFY_STATE, FALSE, eventName.c_str());
	HANDLE hMutex = OpenMutexA(SYNCHRONIZE, FALSE, mutexName.c_str());
	HANDLE hEmpty = OpenSemaphoreA(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, FALSE, emptySemName.c_str());
	HANDLE hFull = OpenSemaphoreA(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, FALSE, fullSemName.c_str());
	
	if (!hReadyEvent || !hMutex || !hEmpty || !hFull) {
		cerr << "Failed to open synchronization objects\n";
		return 1;
	}

	SetEvent(hReadyEvent);

	int tail = 0;
	bool work = true;
	char buffer[note_size] = {0};
	while (work)
	{
		cout << "Choose:" << endl;
		cout << "1.Send to process Receiver note;" << endl;
		cout << "2.Finish work"<< endl;
		int n;
		cin >> n;
		if (!(cin >> n)) 
		{
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			continue;
		}

		cin.ignore(INT_MAX, '\n');
		if (n != 1 && n != 2)
			continue;
		if (n == 1)
		{
			WaitForSingleObject(hEmpty, INFINITE);
			WaitForSingleObject(hMutex, INFINITE);

			fstream fileSend(file_name, ios::in | ios::out | ios::binary);
			cout << "Input your note(max 20 symbols):";
			string note;
			getline(cin, note);
			if (note.size() > note_size) 
				note = note.substr(0, note_size);

			memset(buffer, 0, note_size);
			memcpy(buffer, note.c_str(), note.size());
			fileSend.seekp(tail * note_size, ios::beg);
			fileSend.write(buffer, note_size);
		
			tail = (tail + 1) % (fileSend.tellp() / note_size);

			fileSend.close();

			ReleaseMutex(hMutex);
			ReleaseSemaphore(hFull, 1, NULL);

			cout << "Note sent.\n";
			
		}
		else if (n == 2)
			work = false;
	}

	CloseHandle(hReadyEvent);
	CloseHandle(hMutex);
	CloseHandle(hEmpty);
	CloseHandle(hFull);

	cout << "Sender finished\n";
	return 0;
}