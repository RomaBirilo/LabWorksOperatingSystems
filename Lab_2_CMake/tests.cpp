#include "min_max.h"
#include "average.h"
#include <vector>
#include <ctime>
#include <gtest/gtest.h>
using namespace std;


TEST(AverageTest, WorkChekPositiveInput)
{
	vector<int> testarray{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	EXPECT_EQ(average(testarray),4.5);
	
}

TEST(AverageTest, WorkChekNegativeInput)
{
	vector<int> testarray{ 0, -1, -2, -3, -4, -5, -6, -7, -8, -9 };

	EXPECT_EQ(average(testarray), -4.5);

}

TEST(AverageTest, WorkChekMixedInput)
{
	vector<int> testarray{ 0, -1, 2, -3, 4, -5, 6, -7, 8, -9 };

	EXPECT_EQ(average(testarray), -0.5);

}

TEST(AverageTest, WorkChekZeroInput)
{
	vector<int> testarray{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	EXPECT_EQ(average(testarray),0);

}

TEST(AverageTest, WorkChekOneElement)
{
	vector<int> testarray{1};

	EXPECT_EQ(average(testarray), 1);

}


TEST(AverageTest, WorkChekEmptyArray)
{
	vector<int> testarray;

	EXPECT_EQ(average(testarray), -1);

}



TEST(MinMaxTest, WorkChekPositiveInput)
{
	vector<int> testarray{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	vector <int> res = min_max(testarray);
	EXPECT_EQ(res[0], 0);
	EXPECT_EQ(res[1], 9);

}

TEST(MinMaxTest, WorkChekNegativeInput)
{
	vector<int> testarray{ 0, -1, -2, -3, -4, -5, -6, -7, -8, -9 };
	vector <int> res = min_max(testarray);
	EXPECT_EQ(res[0], -9);
	EXPECT_EQ(res[1], 0);

}

TEST(MinMaxTest, WorkChekMixedInput)
{
	vector<int> testarray{ 0, -1, 2, -3, 4, -5, 6, -7, 8, -9 };

	vector <int> res = min_max(testarray);
	EXPECT_EQ(res[0], -9);
	EXPECT_EQ(res[1], 8);

}

TEST(MinMaxTest, WorkChekZeroInput)
{
	vector<int> testarray{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	vector <int> res = min_max(testarray);
	EXPECT_EQ(res[0], 0);
	EXPECT_EQ(res[1], 0);

}

TEST(MinMaxTest, WorkChekOneElement)
{
	vector<int> testarray{ 1 };

	vector <int> res = min_max(testarray);
	EXPECT_EQ(res[0], 1);
	EXPECT_EQ(res[1], 1);

}

TEST(MinMaxTest, WorkChekEmptyArray)
{
	vector<int> testarray;

	vector <int> res = min_max(testarray);
	EXPECT_EQ(res[0], -1);
	EXPECT_EQ(res[1], -1);

}