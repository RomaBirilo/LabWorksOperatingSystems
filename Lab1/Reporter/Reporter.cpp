#include <iostream>
#include <fstream>
#include <string>
using namespace std;
struct employee
{
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[])
{
	string filename = argv[1];
	string filename_rep = argv[2];
    int hourly_rate = stoi(argv[3]);

	ofstream fout(filename_rep + ".txt");
	fout << "File report " << filename << endl;

    ifstream fin(filename, ios::binary);
    if (!fin) 
    {
        cerr << "Cannot open file!\n";
        
    }

    employee e;
    while (fin.read(reinterpret_cast<char*>(&e), sizeof(e)))
    {
        fout << "Num: " << e.num << ", Name: " << e.name << ", Hours: " << e.hours << ", Salary: " << e.hours* hourly_rate<< endl;
        
    }
    fin.close();
    fout.close();
}