#include "resourcesClient.h"

ostream& operator<<(ostream& out, employee& emp)
{
	out << "Employee's id:";
	out << emp.num;
	out << endl;
	out << "Employee's name:";
	out << emp.name;
	out << endl;
	out << "Work hours:";
	out << emp.hours;
	out << endl;
	return out;
}

istream& operator>>(istream& in, employee& emp)
{
	cout << "Employee's id:";
	in >> emp.num;
	cout << "Employee's name:";
	in >> emp.name;
	cout << "Work hours:";
	in >> emp.hours;
	return in;
}