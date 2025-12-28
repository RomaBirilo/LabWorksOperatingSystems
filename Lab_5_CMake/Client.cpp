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


	DWORD dwBytesRead;
	DWORD dwBytesWritten;
	while(true)
	{
		Request request{};
		Response response{};

		cout << "Choose option:" << endl;
		cout << "1.Modify note" << endl;
		cout << "2.Read note" << endl;
		cout << "3.Finish" << endl;
		cout << "Option:";
		int n;
		cin >> n;

		if (n == 1)
		{
			cout << "Input index of note(start with 1, max " << argv[2] << "):";
			int ind;
			cin >> ind;

			if (ind < 1 || ind > stoi(argv[2]))
			{
				cout << "Please try again" << endl;
				continue;
			}
			

			request.operationType = OperationType::READ;
			request.recordIndex = ind;
			

			WriteFile(hNamedPipe, &request, sizeof(request), &dwBytesWritten, (LPOVERLAPPED)NULL);

			ReadFile(hNamedPipe, &response, sizeof(response), &dwBytesRead, (LPOVERLAPPED)NULL);

			cout << "Note from file:" << endl;
			cout << response.emp;

			cout << "Input what you want to modify(else input -):" << endl;

			cout << "Employee's id:";
			string id;
			cin >> id;
			cout << "Employee's name:";
			string name;
			cin >> name;
			cout << "Work hours:";
			string hours;
			cin >> hours;

			employee emp = response.emp;

			if (id != "-")    
				emp.num = stoi(id);
			if (name != "-")  
				strcpy_s(emp.name, name.c_str());
			if (hours != "-") 
				emp.hours = stod(hours);

			request = {};
			request.operationType = OperationType::WRITE;
			request.recordIndex = ind;
			request.emp = emp;

			WriteFile(hNamedPipe, &request, sizeof(request), &dwBytesWritten, (LPOVERLAPPED)NULL);
			ReadFile(hNamedPipe, &response, sizeof(response), &dwBytesRead, (LPOVERLAPPED)NULL);

		}
		else if (n == 2)
		{
			cout << "Input index of note(start with 1, max " << argv[2] << "):";
			int ind;
			cin >> ind;

			if (ind < 1 || ind > stoi(argv[2]))
			{
				cout << "Please try again" << endl;
				continue;
			}


			request.operationType = OperationType::READ;
			request.recordIndex = ind;


			WriteFile(hNamedPipe, &request, sizeof(request), &dwBytesWritten, (LPOVERLAPPED)NULL);

			ReadFile(hNamedPipe, &response, sizeof(response), &dwBytesRead, (LPOVERLAPPED)NULL);

			cout << "Note from file:" << endl;
			cout << response.emp;
		}
		else if (n == 3)
		{
			request = {};
			request.operationType = OperationType::END;
			request.recordIndex = -1;
			WriteFile(hNamedPipe, &request, sizeof(request), &dwBytesWritten, NULL);
			break;
		}

	}
}