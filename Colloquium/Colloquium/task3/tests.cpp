#include "main.h"
#include "node.h"
#include <gtest/gtest.h>

TEST(NodeCheck, WorkCheckAddFirstElement)
{
	Node* head = nullptr;
	head->push(head,1);
	EXPECT_EQ(head->data, 1);
	EXPECT_EQ(head->next, nullptr);
}

TEST(NodeCheck, WorkCheckAddElements)
{
	Node* head = nullptr;
	int const n = 4;
	for (int i = 0; i < n; i++)
	{
		head->push(head, i);
	}

	EXPECT_EQ(head->data, 0);
	EXPECT_EQ(head->next->data,1);
	EXPECT_EQ(head->next->next->data, 2);
	EXPECT_EQ(head->next->next->next->data, 3);
	EXPECT_EQ(head->next->next->next->next, nullptr);
}


TEST(ReverseListCheck, WorkCheckStandartInput)
{
	Node* head = nullptr;
	int const n = 4;
	for (int i = 0; i < n; i++)
	{
		head->push(head, i);
	}

	head = ReverseList(head);
	EXPECT_EQ(head->data, 3);
	EXPECT_EQ(head->next->data, 2);
	EXPECT_EQ(head->next->next->data, 1);
	EXPECT_EQ(head->next->next->next->data, 0);
	EXPECT_EQ(head->next->next->next->next, nullptr);

}

TEST(ReverseListCheck, WorkCheckOneElement)
{
	Node* head = nullptr;
	head->push(head,1);

	head = ReverseList(head);
	EXPECT_EQ(head->data, 1);
}

TEST(ReverseListCheck, WorkCheckTwiceReverse)
{
	Node* head = nullptr;
	int const n = 4;
	for (int i = 0; i < n; i++)
	{
		head->push(head, i);
	}

	head = ReverseList(head);
	head = ReverseList(head);

	EXPECT_EQ(head->data, 0);
	EXPECT_EQ(head->next->data, 1);
	EXPECT_EQ(head->next->next->data, 2);
	EXPECT_EQ(head->next->next->next->data, 3);
	EXPECT_EQ(head->next->next->next->next, nullptr);

}

