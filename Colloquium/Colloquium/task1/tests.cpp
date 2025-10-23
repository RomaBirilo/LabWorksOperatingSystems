#include "main.h"
#include <vector>
#include <gtest/gtest.h>
using namespace std;

TEST(CountFactorialsTest, WorkChekPositiveNumber)
{
	const int n = 3;
	vector <long long> testArray(n + 1, 0);
	CountFactorials(testArray, n);
	EXPECT_EQ(testArray[0], 1);
	EXPECT_EQ(testArray[1], 1);
	EXPECT_EQ(testArray[2], 2);
	EXPECT_EQ(testArray[3], 6);
	
}


TEST(CountFactorialsTest, WorkChekPositiveNumber2)
{
	const int n = 5;
	vector <long long> testArray(n+1,0);
	CountFactorials(testArray,n);
	EXPECT_EQ(testArray[0],1);
	EXPECT_EQ(testArray[1], 1);
	EXPECT_EQ(testArray[2], 2);
	EXPECT_EQ(testArray[3], 6);
	EXPECT_EQ(testArray[4], 24);
	EXPECT_EQ(testArray[5], 120);
}

TEST(CountFactorialsTest, WorkChekZero)
{
	const int n = 0;
	vector <long long> testArray(n + 1, 0);
	CountFactorials(testArray, n);
	EXPECT_EQ(testArray[0], 1);
}

TEST(CountFactorialsTest, WorkChekNegativeNumber)
{
	const int n = -2;
	vector <long long> testArray;
	EXPECT_THROW(CountFactorials(testArray, n), invalid_argument);
}


