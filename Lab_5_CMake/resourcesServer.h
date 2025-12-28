#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <iostream>

using std::vector;
using std::runtime_error;
using std::to_string;
using std::string;
using std::ios;
using std::fstream;
using std::ofstream;
using std::ifstream;
using std::istream;
using std::ostream;
using std::cout;
using std::cin;
using std::endl;

struct employee
{
	int num; 
	char name[10]; 
	double hours;

};

ostream& operator<< (ostream& out, employee& emp);
istream& operator>> (istream& out, employee& emp);

enum class OperationType : int
{
	READ = 1,
	WRITE = 2,
	END = 3
};

struct Request
{
	OperationType operationType;
	int recordIndex;
	employee emp;
};

struct Response
{
	bool success;       
	employee emp;     
};

struct EmployeeLock
{
	CRITICAL_SECTION cs;
	int readers;         
	bool writer;
};

struct ThreadParams
{
	vector<EmployeeLock>* locks;
	string* fileName;
	int employeesNumber;
};

struct ThreadParamsInd
{
	ThreadParams* params;
	string pipeName;
};

PROCESS_INFORMATION LaunchProcess(char command[]);
HANDLE LaunchNamedPipe(char pipeName[]);
void CloseProcess(PROCESS_INFORMATION process);
void WriteFile(string fileName, int employeesNumber);
void ShowFile(string fileName, int employeesNumber);
bool InitLocks(int employeesNumber, vector<EmployeeLock>& locks);
void CloseLocks(vector<EmployeeLock>& locks);
DWORD WINAPI ClientThread(LPVOID lpParam);
vector <HANDLE> LaunchThreads(int processCount, ThreadParams* params);
void CloseThreads(vector <HANDLE>& threads);
employee Read(EmployeeLock& lock, int index,fstream& fin);
employee ShowNote(int index, fstream& fin);
void Write(EmployeeLock& lock, int index, employee& emp, fstream& fout);