#pragma once
#include <iostream>
#include <windows.h>
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::istream;
using std::ostream;
using std::stoi;
using std::stod;

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