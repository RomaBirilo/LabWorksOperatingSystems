
#pragma once
#include "node.h"
#include <stdexcept>
using namespace std;

Node* ReverseList(Node* head)
{
    if (!head || !head->next)
        return head;
    Node* new_head = ReverseList(head->next);
    if (head->next != nullptr)
    {
        head->next->next = head;
    }
    else
        throw runtime_error("Invalid list structure detected during reversal");
    head->next = nullptr;
    return new_head;
}

