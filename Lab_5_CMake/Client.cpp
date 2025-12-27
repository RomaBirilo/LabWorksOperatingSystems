#include "Client.h"

int main(int argc, char* argv[])
{
	char pipeName[256];
	strcpy(pipeName, argv[1]);

	HANDLE hNamedPipe;
	hNamedPipe = CreateFile(
		pipeName, 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		(LPSECURITY_ATTRIBUTES)NULL, 
		OPEN_EXISTING, 
		0, 
		(HANDLE)NULL 
	);

	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		cout << "Connection with the named pipe failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		
		return 0;
	}

}