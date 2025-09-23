#include <windows.h>
#include <fstream>
#include <string>
#include <gtest/gtest.h>
#include "employee.h"
using namespace std;

int generate_bin_file(const string filenamebin)
{
	ofstream fout(filenamebin, ios::binary);
	if (!fout) 
	{
		cerr << "Cannot open binary file\n";
		return 1;
	}
	employee emp{ 1, "Robert", 12.0 };
	fout.write(reinterpret_cast<char*>(&emp), sizeof(emp));
	fout.close();
	return 0;

}

int generate_txt_file(const string filenamebin, const string filenametxt)
{
	generate_bin_file(filenamebin);

	ifstream fin(filenamebin, ios::binary);
	if (!fin)
	{
		cerr << "Cannot open binary file\n";
		return 1;
	}

	ofstream fout(filenametxt);
	if (!fout)
	{
		cerr << "Cannot open text file\n";
		return 1;
	}
	const double hourly_rate = 4.0;
	
	employee emp;
	while (fin.read(reinterpret_cast<char*>(&emp), sizeof(emp)))
	{
		fout << "Num: " << emp.num << ", Name: " << emp.name << ", Hours: " << emp.hours << ", Salary: " << emp.hours * hourly_rate << endl;

	}
	fin.close();
	fout.close();
	return 0;

}


TEST(CreatorTests, GenarateBinaryFile)
{
	const string filenamebin = "test_binary_file.bin";
	
	int returnval = generate_bin_file(filenamebin);

	EXPECT_EQ(returnval, 0);

	ifstream fin(filenamebin, ios::binary);
	employee emp;
	fin.read(reinterpret_cast<char*>(&emp), sizeof(emp));
	EXPECT_EQ(emp.num, 1);
	EXPECT_STREQ(emp.name, "Robert");
	EXPECT_EQ(emp.hours, 12.0);

	
}

TEST(ReporterTests, GenerateTextFile)
{
	const string filenamebin = "test_binary_file.bin";
	const string filenametxt = "test_text_file.txt";

	int returnval = generate_txt_file(filenamebin, filenametxt);

	EXPECT_EQ(returnval, 0);
}



