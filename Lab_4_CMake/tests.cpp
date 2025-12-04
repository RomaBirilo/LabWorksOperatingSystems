#include <gtest/gtest.h>
#include "BufferClass.h"
#include "Process.h"
#include <windows.h>
#include <cstdio>
#include <string>
#include <optional>
using std::string;
using std::remove;

const string path = "BinFileTest";
const int notes_count = 3;
BufferClass buffer(path, notes_count);

TEST(TestProcess, CreateReadyEventTest)
{
	int idx = 0;
	string baseName = "baseName";
	HANDLE hReadyEvent = createReadyEvent(baseName, idx);

	ASSERT_NE(hReadyEvent, (HANDLE)NULL);

	CloseHandle(hReadyEvent);
}

TEST(TestProcess, ProcessLauncherTest)
{
	string command = "Sender.exe";
	vector<char> cmdbuf(command.begin(), command.end());
	cmdbuf.push_back('\0');
	PROCESS_INFORMATION pi;

	EXPECT_EQ(processLauncher(cmdbuf, pi), true);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

TEST(TestProcess, LaunchSenderProcessTest)
{
	string command = "Sender.exe";
	PROCESS_INFORMATION pi;

	EXPECT_EQ(launchSenderProcess(command, pi), true);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

TEST(TestBufferClass, InitializeTest)
{
	EXPECT_EQ(buffer.initialize(), true);
}

TEST(TestBufferClass, WriteHeadTest)
{
	int head = 1;
	EXPECT_EQ(buffer.writeHead(head), true);
}

TEST(TestBufferClass, WriteTailTest)
{
	int tail = 2;
	EXPECT_EQ(buffer.writeTail(tail), true);
}

TEST(TestBufferClass, ReadHeadTest)
{
	EXPECT_EQ(buffer.readHead(), 1);
}

TEST(TestBufferClass, ReadTailTest)
{
	EXPECT_EQ(buffer.readTail(), 2);
}

TEST(TestBufferClass, WriteRawNoteTest)
{
	int tail = 0;
	string note = "note";
	EXPECT_EQ(buffer.writeRawNote(tail, note), true);

}

TEST(TestBufferClass, ReadRawNoteTest)
{
	int head = 0;
	optional<string> note = buffer.readRawNote(head);
	EXPECT_EQ(note.value(), "note");
}

TEST(TestBufferClass, WriteNoteTest)
{
	int tail = 0;
	string note = "note2";
	EXPECT_EQ(buffer.writeTail(tail), true);
	EXPECT_EQ(buffer.writeNote(note), true);
	EXPECT_EQ(buffer.readTail(), 1);
}

TEST(TestBufferClass, ReadNoteTest)
{
	int head = 0;
	EXPECT_EQ(buffer.writeHead(head), true);
	optional<string> note = buffer.readNote();
	EXPECT_EQ(note.value(), "note2");
	EXPECT_EQ(buffer.readHead(), 1);
}

TEST(TestBufferClass, RemoveTest)
{
	EXPECT_EQ(remove(path.c_str()), 0);
}