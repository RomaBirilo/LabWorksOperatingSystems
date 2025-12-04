#include "BufferClass.h"
#include <fstream>
using namespace std;
BufferClass::BufferClass(string path, int notes_count)
{
	this->path = path;
	this->notes_count = notes_count;
}

bool BufferClass::initialize()
{
	int head = 0;
	int tail = 0;
	ofstream file(path, ios::binary);
	if (!file)
		return false;

	int file_size = notes_count * note_size;
	file.seekp(file_size + header - 1);
	char zero = '\0';
	file.write(&zero, 1);
	file.seekp(0);
	file.write(reinterpret_cast<char*>(&head), sizeof(head));
	file.write(reinterpret_cast<char*>(&tail), sizeof(tail));
	file.close();
	return true;
}

optional<string> BufferClass::readNote()
{
	optional<int> op_head = readHead();
	if (!op_head.has_value())
		return nullopt;
	int head = op_head.value();
	optional<string> note = readRawNote(head);

	if (!note.has_value())
		return nullopt;

	head = (head + 1) % notes_count;

	writeHead(head);
	return note;
}

bool BufferClass::writeNote(string& note)
{
	int tail = readTail();
	if(!writeRawNote(tail, note))
		return false;
	tail = (tail + 1) % notes_count;
	writeTail(tail);
	return true;
}

optional<string> BufferClass::readRawNote(int head)
{
	char buffer[note_size];
	fstream fileRead(path, ios::in | ios::out | ios::binary);
	if (!fileRead)
		return nullopt;
	fileRead.seekg(header + head * note_size, ios::beg);
	fileRead.read(buffer, note_size);
	fileRead.close();

	string note(buffer, buffer + note_size);
	size_t pos = note.find('\0');
	if (pos != string::npos)
		note.erase(pos);
	return note;
}

bool BufferClass::writeRawNote(int tail, string& note)
{
	char buffer[note_size];
	fstream fileSend(path, ios::in | ios::out | ios::binary);
	if (!fileSend)
		return false;
	memset(buffer, 0, note_size);
	memcpy(buffer, note.c_str(), note.size());
	fileSend.seekp(sizeof(int) * 2 + tail * note_size, ios::beg);
	fileSend.write(buffer, note_size);
	fileSend.close();
	return true;
}

optional<int> BufferClass::readHead()
{
	int head;
	fstream fileRead(path, ios::in | ios::out | ios::binary);
	if (!fileRead)
		return nullopt;
	fileRead.seekg(0, ios::beg);
	fileRead.read(reinterpret_cast<char*>(&head), sizeof(head));
	fileRead.close();
	return head;
}

int BufferClass::readTail()
{
	int tail;
	fstream fileSend(path, ios::in | ios::out | ios::binary);
	fileSend.seekg(sizeof(int), ios::beg);
	fileSend.read(reinterpret_cast<char*>(&tail), sizeof(tail));
	fileSend.close();
	return tail;
}

bool BufferClass::writeHead(int head)
{
	fstream fileRead(path, ios::in | ios::out | ios::binary);
	if (!fileRead)
		return false;
	fileRead.seekp(0, ios::beg);
	fileRead.write(reinterpret_cast<char*>(&head), sizeof(head));
	fileRead.close();
	return true;
}

bool BufferClass::writeTail(int tail)
{
	fstream fileSend(path, ios::in | ios::out | ios::binary);
	if (!fileSend)
		return false;
	fileSend.seekp(sizeof(int), ios::beg);
	fileSend.write(reinterpret_cast<char*>(&tail), sizeof(tail));
	fileSend.close();
	return true;
}
