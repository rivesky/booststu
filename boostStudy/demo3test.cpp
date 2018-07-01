#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>
#include <boost/bind.hpp>
int divide_by(int n, int div) {
	return n / div;
}
void test1() {
	std::vector<int> number;
	number.push_back(10);
	number.push_back(20);
	number.push_back(30);
	std::transform(number.begin(), number.end(), number.begin(),boost::bind(divide_by, _1, 2));
	for (auto p = number.begin(); p != number.end(); p++) {
		std::cout << *p << std::endl;
	}
}
void main() {
	test1();
	getchar();
}