#pragma once
#include <string>
#include <optional>
using std::string;
using std::optional;

class BufferClass
{
	int notes_count;
	string path;
    static const int note_size = 20;
    const int header = sizeof(int) * 2;
public:
	BufferClass(string path, int notes_count);
    bool initialize();
    optional<string> readNote();
    bool writeNote(string& note);
    optional<string> readRawNote(int head);
    bool writeRawNote(int tail, string& note);
    optional<int> readHead();
    int readTail();
    bool writeHead(int head);
    bool writeTail(int tail);
};