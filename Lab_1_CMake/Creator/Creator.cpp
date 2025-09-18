
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

using namespace std;

struct employee
{
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[])
{
    cout << "argc=" << argc << "\n";
    for (int i = 0;i < argc;i++) cout << "argv[" << i << "]=\"" << (argv[i] ? argv[i] : "(null)") << "\"\n";

    if (argc < 3) {
        cerr << "Usage: Creator <filename> <count>\n";

    }

    string filename = argv[1];
    int notecount = 0;
    try {
        notecount = stoi(argv[2]);
    }
    catch (const exception& e) {
        cerr << "Bad count: " << e.what() << "\n";

    }

    if (notecount < 0) {
        cerr << "Count must be non-negative\n";

    }

    ofstream fout(filename, ios::binary);
    if (!fout) { cerr << "Cannot open file\n"; return 1; }

    cout << "Will input " << notecount << " employees\n";

    for (int i = 0; i < notecount; ++i) {
        employee emp{};
        cout << "Employee #" << i + 1 << "\n";
        cout << "Input number: ";
        cin >> emp.num;
        cout << "Input name: ";
        cin >> emp.name;
        cout << "Input hours: ";
        cin >> emp.hours;
        fout.write(reinterpret_cast<char*>(&emp), sizeof(emp));
    }

    fout.close();
    cout << "Done\n";

}