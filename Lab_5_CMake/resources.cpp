#include "resources.h"

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

PROCESS_INFORMATION& LaunchProcess()
{
	char command[24] = "Client.exe";
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

HANDLE& LaunchNamedPipe()
{
	HANDLE hNamedPipe;
	hNamedPipe = CreateNamedPipe(
		"\\\\.\\pipe\\MyPipe",
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
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
	PROCESS_INFORMATION pi;
	HANDLE hNamedPipe;
	try
	{
		pi = LaunchProcess();
		hNamedPipe = LaunchNamedPipe();
	}
	catch (const runtime_error& ex)
	{
		throw runtime_error(ex.what());
	}
	
	if (!ConnectNamedPipe(hNamedPipe, (LPOVERLAPPED)NULL))
	{
		DWORD err = GetLastError();
		CloseHandle(hNamedPipe);
		throw runtime_error("The connection failed.Win32 error code: " + to_string(err));
		return 0;
	}

	ThreadParams* p = static_cast<ThreadParams*>(lpParam);
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
	CloseProcess(pi);
	return 0;
}

vector<HANDLE>& LaunchThreads(int processCount, ThreadParams* params)
{
	vector <HANDLE> threads(processCount);
	
	for (size_t i = 0; i < processCount; i++)
	{
		DWORD threadId;
		HANDLE hThread = CreateThread(NULL, 0, ClientThread, params, 0, &threadId);
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

employee& Read(EmployeeLock& lock, int index, fstream& fin)
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


/*#include <windows.h>
#include <vector>
#include <iostream>

using namespace std;

// ================== ДАННЫЕ ==================

struct employee
{
    int num;
    char name[10];
    double hours;
};

// ================== БЛОКИРОВКА ОДНОЙ ЗАПИСИ ==================

struct EmployeeLock
{
    CRITICAL_SECTION cs; // защита служебных данных
    int readers;         // количество читающих потоков
    bool writer;         // есть ли писатель
};

// ================== ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ==================

HANDLE hFile = INVALID_HANDLE_VALUE;
vector<EmployeeLock> locks;
int employeeCount = 0;

// ================== ИНИЦИАЛИЗАЦИЯ ==================

bool InitStorage(const wchar_t* filename, int count)
{
    employeeCount = count;

    hFile = CreateFileW(
        filename,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE)
    {
        cout << "Failed to open file\n";
        return false;
    }

    // инициализация блокировок
    locks.resize(employeeCount);
    for (int i = 0; i < employeeCount; i++)
    {
        InitializeCriticalSection(&locks[i].cs);
        locks[i].readers = 0;
        locks[i].writer = false;
    }

    return true;
}

// ================== ЧТЕНИЕ ==================

bool ReadEmployee(int index, employee& emp)
{
    if (index < 0 || index >= employeeCount)
        return false;

    EmployeeLock& lock = locks[index];

    // 1. Ждём, если кто-то пишет
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

    // 2. Читаем запись
    DWORD offset = index * sizeof(employee);
    SetFilePointer(hFile, offset, NULL, FILE_BEGIN);

    DWORD bytesRead = 0;
    BOOL ok = ReadFile(hFile, &emp, sizeof(employee), &bytesRead, NULL);

    // 3. Завершаем чтение
    EnterCriticalSection(&lock.cs);
    lock.readers--;
    LeaveCriticalSection(&lock.cs);

    return ok && bytesRead == sizeof(employee);
}

// ================== ЗАПИСЬ ==================

bool WriteEmployee(int index, const employee& emp)
{
    if (index < 0 || index >= employeeCount)
        return false;

    EmployeeLock& lock = locks[index];

    // 1. Ждём, пока никто не читает и не пишет
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

    // 2. Записываем запись
    DWORD offset = index * sizeof(employee);
    SetFilePointer(hFile, offset, NULL, FILE_BEGIN);

    DWORD bytesWritten = 0;
    BOOL ok = WriteFile(hFile, &emp, sizeof(employee), &bytesWritten, NULL);

    // 3. Завершаем запись
    EnterCriticalSection(&lock.cs);
    lock.writer = false;
    LeaveCriticalSection(&lock.cs);

    return ok && bytesWritten == sizeof(employee);
}

// ================== ЗАВЕРШЕНИЕ ==================

void CloseStorage()
{
    for (auto& l : locks)
        DeleteCriticalSection(&l.cs);

    if (hFile != INVALID_HANDLE_VALUE)
        CloseHandle(hFile);
}


}
*/