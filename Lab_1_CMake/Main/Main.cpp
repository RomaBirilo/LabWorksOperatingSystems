
#include "Main.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>
using namespace std;

struct employee
{
    int num;
    char name[10];
    double hours;
};



int main()
{

    wstring n;
    wstring filename;
    cout << "Input name of binary file:";
    getline(wcin, filename);
    cout << "Input a count of notes in file:";
    getline(wcin, n);
    wstring command = L"\"Creator.exe\" \"" + filename + L"\" " + n;

    wchar_t cmdLine[1024];
    wcscpy_s(cmdLine, command.c_str());

    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);



    if (!CreateProcessW(NULL,
        cmdLine,
        NULL,
        NULL,
        FALSE,
        CREATE_NEW_CONSOLE,
        NULL,
        NULL,
        &si,
        &pi)) {
        std::cerr << "Failed to start Creator.exe. : " << GetLastError() << endl;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    cout << endl;
    cout << "Binary file notes:" << endl;
    ifstream fin(filename, ios::binary);
    employee e;
    int i = 1;
    while (fin.read(reinterpret_cast<char*>(&e), sizeof(e)))
    {
        cout << "Employee #" << i << endl;
        cout << "Num: " << e.num << ", Name: " << e.name << ", Hours: " << e.hours << endl;
        cout << endl;
        i++;
    }
    fin.close();



    cout << "Enter the report file name:";
    wstring hourly_rate;
    wstring filename_rep;
    getline(wcin, filename_rep);
    cout << "Enter the hourly rate:";
    getline(wcin, hourly_rate);

    wstring command_rep = L"\"Reporter.exe\" \"" + filename + L"\" " + L"\"" + filename_rep + L"\" " + hourly_rate;

    wchar_t cmdLine_rep[1024];
    wcscpy_s(cmdLine_rep, command_rep.c_str());

    STARTUPINFOW si2 = { sizeof(si2) };
    PROCESS_INFORMATION pi2;
    ZeroMemory(&si2, sizeof(si2));
    si2.cb = sizeof(si2);

    if (!CreateProcessW(NULL,
        cmdLine_rep,
        NULL,
        NULL,
        FALSE,
        CREATE_NEW_CONSOLE,
        NULL,
        NULL,
        &si2,
        &pi2)) {
        std::cerr << "Failed to start Reporter.exe. : " << GetLastError() << endl;
    }

    WaitForSingleObject(pi2.hProcess, INFINITE);

    CloseHandle(pi2.hProcess);
    CloseHandle(pi2.hThread);


    ifstream fin_rep(filename_rep + L".txt");
    string temp;
    cout << endl;
    cout << "Notes in reporter file:" << endl;
    while (getline(fin_rep, temp))
    {
        cout << temp << endl;

    }
    fin_rep.close();
    cout << "Done:)";


}