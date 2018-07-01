#include <boost/timer.hpp>
#include <iostream>
using namespace std;
void main1() {
	boost::timer tm;
	cout << "hello" << endl;
	cout << tm.elapsed() << endl;
	getchar();
}