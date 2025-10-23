
#include "main.h"
#include "node.h"
#include <iostream>
#include <stdexcept>
using namespace std;

int main()
{
	try
	{
		cout << "Input size of list:";
		int n;
		cin >> n;
		if (n < 0)
			throw invalid_argument("size of list must be > 0");
		cout << "Input list:";
		Node* head = nullptr;
		int num;
		for (int i = 0; i < n; i++)
		{
			cin >> num;
			head->push(head, num);
		}

		head = ReverseList(head);

		Node* curr = head;
		while (curr) {
			cout << curr->data << " ";
			curr = curr->next;
		}
	}
	catch (const invalid_argument& er)
	{
		cout << er.what();
	}
	catch (const runtime_error& er)
	{
		cout << er.what();
	}

}
