/*
��������
��ָ��Щ���Ա����뵽�������������Ǵ������������ص�һ�ຯ����
�������ڴ����������BoostC++���У�Boost.Bind�����滻����c++��׼��
������std::bind1st��std::bind2nd������������Boost.Function���ṩ��һ�����ڷ�װ����ָ����ࡣ
���Boost.Lambda��������һ�ִ������������ķ�����
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
Lambda�������������Ѿ��ڶ���Ԥ���д��ڣ��ú�����Ŀ�������������գ��Ӷ��������
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
boostlambda�����ں����ڲ�ʹ��if���
*/
void boostlambdaif() {
	std::vector<int >v;
	v.push_back(3);
	v.push_back(5);
	v.push_back(0);
	std::for_each(v.begin(), v.end(), boost::lambda::if_then(boost::lambda::_1 > 1, std::cout << boost::lambda::_1 << "\n"));
}
/*
Ϊ�˷�װ����ָ�룬boost::function�ṩ�˸��࣬�÷�����
ע��boost::function�ĺ������Ͳ���Ҫ��ȷƥ��
�������fΪ����һ����������������ô���׳�һ��boost::bad_function_call�쳣
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
��ֵ0����һ��boost::function ���͵ĺ���ָ�룬�����ͷŵ�ǰ�����ĺ������ͷ�֮���ڽ��е��ã���ô
�ᵼ��boost::bad_function_call�쳣���׳���Ҫ���һ������ָ���Ƿ񱻸�ֵĳ������������ʹ��empty()����
operator bool()������
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
Boost.refͨ����boost::bindһ��ʹ�ã���Ҫ����boost::bind()�ĺ�����������һ�����ò���ʱ��Boost.Ref��
�Ƚ���Ҫ�ˣ�����boost::bind()�Ḵ�����Ĳ������������ñ����ر���.
ע�⣺�����иú�����Ҫ����һ�����������������ӡ��Ϣ����Ϊ����boost::bind()�Ĳ������������ݷ�ʽ�ģ�����std::cout����ֱ��ʹ�ã���˿���ʹ��boost::ref�������÷�ʽ�Ĵ���
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
Boost.Bind����c++��׼�е�std::bind1st()��std::bind2nd()ģ�庯�����ṩ��һ�����ƣ�����Щ������
�������������Ĳ���һ��ʹ�ã��Ϳ��Եõ�ָ��ǩ���ĺ�����
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
	//��ʵ����ת����binary_function
	std::for_each(v.begin(), v.end(), std::bind1st(add(), 10));

}
void add2(int i, int j) {
	std::cout << i + j << std::endl;
}
/*
_1 ����Ϊռλ��(placeholder)�������� Boost.Bind�� ���� _1��Boost.Bind �������� _2 �� _3�� ͨ��ʹ����Щռλ����boost::bind() ���Ա�ΪһԪ����Ԫ����Ԫ�ĺ����� ���� _1, boost::bind() �����һ��һԪ���� - ��ֻҪ��һ�������ĺ����� ���Ǳ���ģ���Ϊ std::for_each() ����Ҫ��һ��һԪ������Ϊ�������������
*/
void boostbind() {
	std::vector<int> v;
	v.push_back(1);
	v.push_back(3);
	v.push_back(2);
	//��ʵ����ת����binary_function

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