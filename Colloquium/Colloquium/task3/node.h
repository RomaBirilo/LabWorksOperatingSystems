#pragma once

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
	void push(Node*& head, int data)
	{
		if (head == nullptr)
			head = new Node(data);
		else
		{
			Node* current = head;
			while (current->next != nullptr)
				current = current->next;
			current->next = new Node(data);
		}

	}
};