#include "main.h"
#include <vector>
#include <gtest/gtest.h>

TEST(RemoveRepetitionsTest, WorkChekStandartInput)
{
	vector <int> testArray{1,2,2,5,8,8,8};
	vector <int> supposeArray{1,2,5,8};
	EXPECT_EQ(RemoveRepetitions(testArray), supposeArray);
}

TEST(RemoveRepetitionsTest, WorkChekNegativeInput)
{
	vector <int> testArray{ -1,-2,-2,-5,-8,-8,-8 };
	vector <int> supposeArray{ -1,-2,-5,-8};
	EXPECT_EQ(RemoveRepetitions(testArray), supposeArray);
}

TEST(RemoveRepetitionsTest, WorkChekPositiveAndNegative)
{
	vector <int> testArray{ 1,-2,2,2,5,-8,-8,8 };
	vector <int> supposeArray{ 1,-2,2,5,-8,8 };
	EXPECT_EQ(RemoveRepetitions(testArray), supposeArray);
}

TEST(RemoveRepetitionsTest, WorkChekOneElement)
{
	vector <int> testArray{ 1 };
	vector <int> supposeArray{ 1 };
	EXPECT_EQ(RemoveRepetitions(testArray), supposeArray);
}

TEST(RemoveRepetitionsTest, WorkChekSameElement)
{
	vector <int> testArray{ 1,1,1,1,1};
	vector <int> supposeArray{ 1 };
	EXPECT_EQ(RemoveRepetitions(testArray), supposeArray);
}

TEST(RemoveRepetitionsTest, WorkChekEmptyArray)
{
	vector <int> testArray{ };
	vector <int> supposeArray{  };
	EXPECT_EQ(RemoveRepetitions(testArray), supposeArray);
}

