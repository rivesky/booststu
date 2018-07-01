/*
函数对象
是指哪些可以被传入到其他函数或者是从其他函数返回的一类函数。
这里用于处理函数对象的BoostC++库中，Boost.Bind可以替换来自c++标准的
著名的std::bind1st和std::bind2nd（）函数，而Boost.Function则提供了一个用于封装函数指针的类。
最后Boost.Lambda则引入了一种创建匿名函数的方法。
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <cstdlib>
#include <cstring>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/if.hpp>
void bind1ts();
void bind2nd();
void boostbind();
void sortboostbind();
void sortboostbindshengxu();
void stdsort();
void boostref();
void functionboost();
void boostfunction0();
void stdbiaozhun();
void lambdaboost();
void boostlambdaif();
void main3() {
	boostlambdaif();
	getchar();
}
/*
Lambda，匿名函数，已经在多种预演中存在，该函数的目标是令代码更紧凑，从而容易理解
*/
void print1(int i) {
	std::cout << i << std::endl;
}
void stdbiaozhun() {
	std::vector<int> v;
	v.push_back(1);
	v.push_back(3);
	v.push_back(2);

	std::for_each(v.begin(), v.end(), print1);
}
void lambdaboost() {
	std::vector<int> v;
	v.push_back(3);
	v.push_back(2);
	v.push_back(9);
	std::for_each(v.begin(), v.end(), std::cout << boost::lambda::_1 << "\n");
}
/*
boostlambda允许在函数内部使用if语句
*/
void boostlambdaif() {
	std::vector<int >v;
	v.push_back(3);
	v.push_back(5);
	v.push_back(0);
	std::for_each(v.begin(), v.end(), boost::lambda::if_then(boost::lambda::_1 > 1, std::cout << boost::lambda::_1 << "\n"));
}
/*
为了封装函数指针，boost::function提供了该类，用法如下
注意boost::function的函数类型不需要精确匹配
并且如果f为赋予一个函数而被调用那么会抛出一个boost::bad_function_call异常
*/
void functionboost() {
	boost::function<int(const char*)> f = std::atoi;
	int(*f2)(const char*) = std::atoi;
	std::cout << f("1609") << std::endl;
	std::cout << f2("1610") << std::endl;
	f = std::strlen;
	std::cout << f("1607") << std::endl;
	try {
		boost::function<int(const char*)> f;
		f("");
	}
	catch (boost::bad_function_call &ex) {
		std::cout << ex.what() << std::endl;
	}
}
/*
将值0赋给一个boost::function 类型的函数指针，将会释放当前所赋的函数。释放之后在进行调用，那么
会导致boost::bad_function_call异常被抛出。要检查一个函数指针是否被赋值某个函数，可以使用empty()或者
operator bool()操作符
*/
struct world {
	void hello(std::ostream &os) {
		os << "Hello,world!" << std::endl;
	}
};
void boostfunction0() {
	boost::function<void(world*, std::ostream&)> f = &world::hello;
	world w;
	f(&w, boost::ref(std::cout));
}
/*
Boost.ref通常与boost::bind一起使用，当要用于boost::bind()的函数带有至少一个引用参数时，Boost.Ref就
比较重要了，由于boost::bind()会复制它的参数，所以引用必须特别处理.
注意：例子中该函数需要传入一个流对象的引用来打印信息，因为传给boost::bind()的参数是以至传递方式的，所以std::cout不能直接使用，因此可以使用boost::ref进行引用方式的传递
*/
void addboostref(int i, int j, std::ostream &os) {
	os << i + j << std::endl;
}
void boostref() {
	std::vector<int> v;
	v.push_back(1);
	v.push_back(3);
	v.push_back(2);
	std::for_each(v.begin(), v.end(), boost::bind(addboostref, 10, _1, boost::ref(std::cout)));
}
/*
Boost.Bind简化了c++标准中的std::bind1st()和std::bind2nd()模板函数所提供的一个机制，将这些函数与
几乎不限数量的参数一起使用，就可以得到指定签名的函数。
*/
void print(int i) {
	std::cout << i << std::endl;
}
void bind1ts() {
	std::vector<int> v;
	v.push_back(1);
	v.push_back(3);
	v.push_back(2);
	std::for_each(v.begin(), v.end(), print);
}
class add :public std::binary_function<int, int, void> {
public:
	void operator()(int i, int j)const {
		std::cout << i + j << std::endl;

	}
};
void bind2nd() {
	std::vector<int> v;
	v.push_back(1);
	v.push_back(3);
	v.push_back(2);
	//其实就是转成了binary_function
	std::for_each(v.begin(), v.end(), std::bind1st(add(), 10));

}
void add2(int i, int j) {
	std::cout << i + j << std::endl;
}
/*
_1 被称为占位符(placeholder)，定义于 Boost.Bind。 除了 _1，Boost.Bind 还定义了 _2 和 _3。 通过使用这些占位符，boost::bind() 可以变为一元、二元或三元的函数。 对于 _1, boost::bind() 变成了一个一元函数 - 即只要求一个参数的函数。 这是必需的，因为 std::for_each() 正是要求一个一元函数作为其第三个参数。
*/
void boostbind() {
	std::vector<int> v;
	v.push_back(1);
	v.push_back(3);
	v.push_back(2);
	//其实就是转成了binary_function

	std::for_each(v.begin(), v.end(), boost::bind(add2, 10, _1));
}
bool compare(int i, int j) {
	return i > j;
}
void sortboostbind() {
	std::vector<int> v;
	v.push_back(2);
	v.push_back(1);
	v.push_back(4);
	std::sort(v.begin(), v.end(), boost::bind(compare, _1, _2));
}
void stdsort() {
	std::vector<int> v;
	v.push_back(2);
	v.push_back(1);
	v.push_back(4);
	std::sort(v.begin(), v.end(), compare);
}
void sortboostbindshengxu() {
	std::vector<int> v;
	v.push_back(2);
	v.push_back(1);
	v.push_back(4);
	std::sort(v.begin(), v.end(), boost::bind(compare, _2, _1));
}