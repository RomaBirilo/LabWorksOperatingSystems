#include "resourcesServer.h"

ostream& operator<<(ostream& out, employee& emp)
{
	out << "Employee's id:";
	out << emp.num;
	out << endl;
	out << "Employee's name:";
	out << emp.name;
	out << endl;
	out << "Work hours:";
	out << emp.hours;
	out << endl;	
	return out;
}

istream& operator>>(istream& in, employee& emp)
{
	cout << "Employee's id:";
	in >> emp.num;
	cout << "Employee's name:";
	in >> emp.name;
	cout << "Work hours:";
	in >> emp.hours;
	return in;
}

PROCESS_INFORMATION LaunchProcess(char command[])
{
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);
	if (!CreateProcessA(
		NULL,
		command,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi))
	{
		DWORD err = GetLastError();
		throw runtime_error("Failed to create process. Win32 error code: " + to_string(err));
	}
	return pi;
}

HANDLE LaunchNamedPipe(char pipeName[])
{
	HANDLE hNamedPipe;
	hNamedPipe = CreateNamedPipe(
		pipeName,
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_WAIT,
		1,
		0,
		0,
		INFINITE,
		(LPSECURITY_ATTRIBUTES)NULL
	);

	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		DWORD err = GetLastError();
		throw runtime_error("Failed to create named pipe. Win32 error code: " + to_string(err));
	}

	return hNamedPipe;
}

void CloseProcess(PROCESS_INFORMATION process)
{
	WaitForSingleObject(process.hProcess, 1000);
	CloseHandle(process.hProcess);
	CloseHandle(process.hThread);
}

void WriteFile(string fileName, int employeesNumber)
{
	ofstream fout(fileName, ios::binary);
	for (size_t i = 0; i < employeesNumber; i++)
	{
		cout << "Input information for employee " << i + 1 << ":" << endl;
		employee emp;
		cin >> emp;
		fout.write(reinterpret_cast<char*>(&emp), sizeof(emp));
	}
	fout.close();
	cout << endl;
}

void ShowFile(string fileName, int employeesNumber)
{
	ifstream fin(fileName, ios::binary);
	for (size_t i = 0; i < employeesNumber; i++)
	{
		employee emp;
		fin.read(reinterpret_cast<char*>(&emp), sizeof(emp));
		cout << "Employee #" << i + 1 << ":" << endl;
		cout << emp;
		cout << endl;
	}
}

bool InitLocks(int employeesNumber, vector<EmployeeLock>& locks)
{
	for (int i = 0; i < employeesNumber; i++)
	{
		InitializeCriticalSection(&locks[i].cs);
		locks[i].readers = 0;
		locks[i].writer = false;
	}
	return true;
}

void CloseLocks(vector<EmployeeLock>& locks)
{
	for (size_t i = 0; i < locks.size(); i++)
	{
		DeleteCriticalSection(&locks[i].cs);
	}
}

DWORD __stdcall ClientThread(LPVOID lpParam)
{
	ThreadParamsInd* params = static_cast<ThreadParamsInd*>(lpParam);
	ThreadParams* p = params->params;
	string cmd = "Client.exe \"" + params->pipeName + "\"";
	char pipeName[256];
	char command[256];
	strcpy_s(pipeName, sizeof(pipeName), params->pipeName.c_str());
	strcpy_s(command, sizeof(command), cmd.c_str());
	PROCESS_INFORMATION pi;
	HANDLE hNamedPipe;
	try
	{
		hNamedPipe = LaunchNamedPipe(pipeName);
		pi = LaunchProcess(command);
		bool ok = ConnectNamedPipe(hNamedPipe, (LPOVERLAPPED)NULL);
		if (!ok)
		{
			DWORD err = GetLastError();
			CloseHandle(hNamedPipe);
			throw runtime_error("The connection failed.Win32 error code: " + to_string(err));
			return 0;
		}
	}
	catch (const runtime_error& ex)
	{
		throw runtime_error(ex.what());
	}
	
	cout << "Client connected successfully!" << endl;
	
	vector<EmployeeLock>& locks = *(p->locks);
	string& fileName = *(p->fileName);
	int employeesNumber = p->employeesNumber;

	DWORD dwBytesRead;
	DWORD dwBytesWritten;
	Request request;
	Response response;
	fstream file(fileName, ios::in | ios::out | ios::binary);

	while (true)
	{
		ReadFile(hNamedPipe, &request, sizeof(request), &dwBytesRead, (LPOVERLAPPED)NULL);

		if (request.recordIndex < 0 || request.recordIndex >= employeesNumber)
			break;

		if (request.operationType == "READ")
		{
			employee emp;
			emp = Read(locks[request.recordIndex - 1], request.recordIndex - 1, file);

			response.success = true;
			response.emp = emp;

			WriteFile(hNamedPipe, &response, sizeof(response), &dwBytesWritten, (LPOVERLAPPED)NULL);

		}
		else if (request.operationType == "WRITE")
		{
			Write(locks[request.recordIndex - 1], request.recordIndex - 1, request.emp, file);
			response.success = true;
		}
		else if (request.operationType == "END")
			break;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseProcess(pi);
	return 0;
}

vector<HANDLE> LaunchThreads(int processCount, ThreadParams* params)
{
	vector <HANDLE> threads(processCount);
	
	for (size_t i = 0; i < processCount; i++)
	{
		DWORD threadId;
		ThreadParamsInd* paramsInd = new ThreadParamsInd;
		string pipeName = "\\\\.\\pipe\\MyPipe";
		pipeName += to_string(i);
		paramsInd->params = params;
		paramsInd->pipeName = pipeName;
		HANDLE hThread = CreateThread(NULL, 0, ClientThread, paramsInd, 0, &threadId);
		if (hThread == NULL) 
		{
			DWORD err = GetLastError();
			throw runtime_error("Failed to create thread. Win32 error code: " + to_string(err));
		}
		threads[i] = hThread;
	}
	return threads;
}

void CloseThreads(vector<HANDLE>& threads)
{
	for (size_t i = 0; i < threads.size(); i++)
	{
		CloseHandle(threads[i]);
	}
}

employee Read(EmployeeLock& lock, int index, fstream& fin)
{
	while (true)
	{
		EnterCriticalSection(&lock.cs);
		if (!lock.writer)
		{
			lock.readers++;
			LeaveCriticalSection(&lock.cs);
			break;
		}
		LeaveCriticalSection(&lock.cs);
		Sleep(1);
	}

	employee emp;
	int offset = index * sizeof(employee);
	fin.seekg(offset, ios::beg);
	fin.read(reinterpret_cast<char*>(&emp), sizeof(emp));


	EnterCriticalSection(&lock.cs);
	lock.readers--;
	LeaveCriticalSection(&lock.cs);

	return emp;
}

void Write(EmployeeLock& lock, int index, employee& emp, fstream& fout)
{
	while (true)
	{
		EnterCriticalSection(&lock.cs);
		if (!lock.writer && lock.readers == 0)
		{
			lock.writer = true;
			LeaveCriticalSection(&lock.cs);
			break;
		}
		LeaveCriticalSection(&lock.cs);
		Sleep(1);
	}

	
	int offset = index * sizeof(employee);
	fout.seekp(offset, ios::beg);
	fout.write(reinterpret_cast<char*>(&emp), sizeof(emp));

	EnterCriticalSection(&lock.cs);
	lock.writer = false;
	LeaveCriticalSection(&lock.cs);

}
