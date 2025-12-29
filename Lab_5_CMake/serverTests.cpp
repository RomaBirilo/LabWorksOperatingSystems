#include "resourcesServer.h"
#include <gtest/gtest.h>
#include <cstdio>
#include <sstream>

using std::remove;
using std::istringstream;
using std::ostringstream;
using std::streambuf;

string path = "BinFileTest";
vector<EmployeeLock> locks(1);


DWORD WINAPI PipeClientThread(LPVOID param)
{
    const char* pipeName = static_cast<const char*>(param);

    HANDLE hClient = CreateFileA(
        pipeName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    EXPECT_NE(hClient, INVALID_HANDLE_VALUE);

    if (hClient != INVALID_HANDLE_VALUE)
        CloseHandle(hClient);

    return 0;
}

TEST(LaunchProcessTests, ProcessIsCreatedTest)
{
    char command[] = "Client.exe";

    PROCESS_INFORMATION pi = LaunchProcess(command);

    EXPECT_NE(pi.hProcess, nullptr);
    EXPECT_NE(pi.hThread, nullptr);

    DWORD waitResult = WaitForSingleObject(pi.hProcess, 0);
    EXPECT_EQ(waitResult, WAIT_TIMEOUT);

    TerminateProcess(pi.hProcess, 0);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

TEST(LaunchProcessTests, ProcessThrowsTest)
{
    char command[] = "this_process_does_not_exist.exe";

    EXPECT_THROW(LaunchProcess(command),runtime_error);
}

TEST(CloseProcessTest, ProcessClosedTest)
{
    char command[] = "Client.exe";

    PROCESS_INFORMATION pi = LaunchProcess(command);

    EXPECT_TRUE(CloseProcess(pi));

}

TEST(LaunchNamedPipeTests, NamedPipeIsCreatedTest)
{
    char pipeName[] = "\\\\.\\pipe\\TestPipe";

    HANDLE hPipe = LaunchNamedPipe(pipeName);

    EXPECT_NE(hPipe, INVALID_HANDLE_VALUE);
    EXPECT_NE(hPipe, nullptr);

    CloseHandle(hPipe);
}

TEST(LaunchNamedPipeTests, NamedPipeThrowsTest)
{
    char pipeName[] = "invalid_pipe_name";

    EXPECT_THROW(LaunchNamedPipe(pipeName), runtime_error);
}

TEST(LaunchNamedPipeTests, CanConnectToNamedPipeTest)
{
    char pipeName[] = "\\\\.\\pipe\\TestPipe";

    HANDLE hPipe = LaunchNamedPipe(pipeName);
    ASSERT_NE(hPipe, INVALID_HANDLE_VALUE);

    DWORD threadId;
    HANDLE hClientThread = CreateThread(
        NULL,
        0,
        PipeClientThread,
        pipeName,
        0,
        &threadId
    );
    ASSERT_NE(hClientThread, nullptr);

    BOOL connected = ConnectNamedPipe(hPipe, NULL);
    if (!connected && GetLastError() == ERROR_PIPE_CONNECTED)
        connected = TRUE;

    EXPECT_TRUE(connected);

    WaitForSingleObject(hClientThread, INFINITE);

    CloseHandle(hClientThread);
    CloseHandle(hPipe);
}


TEST(CloseNamedPipeTest, NamedPipeClosedTest)
{
    char pipeName[] = "\\\\.\\pipe\\TestPipe";

    HANDLE hPipe = LaunchNamedPipe(pipeName);

    EXPECT_TRUE(CloseNamedPipe(hPipe));

}

TEST(WriteFileTest, IsInputCorrectTest)
{
    istringstream input("1\nJohn\n40\n2\nAlice\n35\n");
    ostringstream output;

    streambuf* cin_backup = std::cin.rdbuf(input.rdbuf());
    streambuf* cout_backup = std::cout.rdbuf(output.rdbuf());

    bool result = WriteFile(path, 2);

    cin.rdbuf(cin_backup); 
    cout.rdbuf(cout_backup);

    EXPECT_TRUE(result);

}

TEST(ShowFileTest, IsOutputCorrectTest)
{
    ostringstream output;
    streambuf* cout_backup = std::cout.rdbuf(output.rdbuf());

    ShowFile(path, 2);

    cout.rdbuf(cout_backup);

    string expected_output =
        "Employee #1:\n"
        "Employee's id:1\n"
        "Employee's name:John\n"
        "Work hours:40\n\n"
        "Employee #2:\n"
        "Employee's id:2\n"
        "Employee's name:Alice\n"
        "Work hours:35\n\n";

    EXPECT_EQ(output.str(), expected_output);
}

TEST(InitLocksTest, CorrectInitializationTest)
{
    EXPECT_TRUE(InitLocks(1, locks));

    EXPECT_EQ(locks[0].readers,0);
    EXPECT_FALSE(locks[0].writer);

}

TEST(WriteTest, CorrectInputTest)
{
    employee emp{1, "Bob", 40.0};
    InitLocks(1,locks);
    EXPECT_TRUE(Write(locks[0], 0, emp, path));

}

TEST(ReadTest, CorrectInputTest)
{
    InitLocks(1, locks);
    employee emp;
    emp = Read(locks[0], 0, path);
    EXPECT_EQ(emp.num,1);
    EXPECT_STREQ(emp.name,"Bob");
    EXPECT_EQ(emp.hours, 40.0);
}

TEST(CloseLocksTest, CorrectCloseTest)
{
    EXPECT_TRUE(CloseLocks(locks));
}

TEST(DelteTestFileTest, RemoveTest)
{
    EXPECT_EQ(remove(path.c_str()), 0);
}

