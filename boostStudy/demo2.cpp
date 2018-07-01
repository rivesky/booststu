#include <boost/shared_ptr.hpp>
#include<vector>
#include <iostream>
#include <windows.h>
#include <boost/shared_array.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <atlbase.h>
#include <boost/ptr_container/ptr_vector.hpp>
void test();
void testsharedptrdeleter();
void sharedsarrayptr();
void weakptr();
void intrusiveptr();
void ptrvector();
void main2() {

   
	intrusiveptr();
	getchar();
}
/*
指针容器，可以通过普通的vecotr进行存放，但是考虑到会引起引用计数的改变，因此有单独的针对性的指针容器进行存放。
*/
/*
介入式指针，大体上，介入式指针的工作方式和共享指针完全一样。boost::shared_ptr在内部记录着引用到某个对象的共享指针的数量，可是对介入
式指针来说，程序员就得自己来做记录。对于框架对象来说这就特别有用，因为它们记录着自身被i引用的次数。
*/
void intrusive_ptr_add_ref(IDispatch *p) {
	p->AddRef();
}
void intrusive_ptr_release(IDispatch *p) {
	p->Release();
}
void check_windows_foler() {
	CLSID clsid;
	CLSIDFromProgID(CComBSTR("Scripting.FileSystemObject"), &clsid);
	void *p;
	CoCreateInstance(clsid, 0, CLSCTX_INPROC_SERVER, __uuidof(IDispatch), &p);
	boost::intrusive_ptr<IDispatch> disp(static_cast<IDispatch*>(p));
	CComDispatchDriver dd(disp.get());
	CComVariant arg("C:\\Windows");
	CComVariant ret(false);
	dd.Invoke1(CComBSTR("FolderExists"), &arg, &ret);
	std::cout << (ret.boolVal != 0) << std::endl;
}
void intrusiveptr() {
	CoInitialize(0);
	check_windows_foler();
	CoUninitialize();
}
/*
弱指针只有配合共享指针一起使用时才有意义。
*/
DWORD WINAPI reset(LPVOID p) {
	boost::shared_ptr<int> *sh = static_cast<boost::shared_ptr<int>*>(p);
	sh->reset();
	return 0;
}
DWORD WINAPI print(LPVOID p) {
	boost::weak_ptr<int> *w = static_cast<boost::weak_ptr<int>*>(p);
	boost::shared_ptr<int> sh = w->lock();
	if (sh)
		std::cout << *sh << std::endl;
	return 0;

}
/*
boost::weak_ptr必定总是通过boost::shared_ptr来初始化的。一旦初始化后，它基本上只提供一个有用的方法：lock().
该方法返回的boost::shared_ptr与用来初始化弱指针的共享指针共享所有权。如果这个共享指针不含有任何对象。返回的共享指针也将是空的。
注意当函数需要一个由共享指针掌管着这个对象，函数就可以使用该对象。如果共享指针复位了。就算函数里能得到一个共享指针，对象也不存在了。
*/
void weakptr() {
	boost::shared_ptr<int> sh(new int(99));
	boost::weak_ptr<int> w(sh);
	HANDLE thread[2];
	thread[0] = CreateThread(0, 0, reset, &sh, 0, 0);
	thread[1] = CreateThread(0, 0, print, &w, 0, 0);
	WaitForMultipleObjects(2, thread, TRUE, INFINITE);
}
/*
共享数组的行为类似与共享指针，关键不同在于共享数组在析构时，默认使用delte[]操作符来释放所含的对象。因为这个操作符只能用于数组对象。
共享数组必须通过动态分配的数组的地址来初始化
*/
void sharedsarrayptr() {
	boost::shared_array<int> i1(new int[2]);
	boost::shared_array<int> i2(i1);
	i1[0] = 1;
	std::cout << i2[0] << std::endl;
}


/*
共享智能指针boost::shared_ptr基本上类似于boost::scoped_ptr.关键不同之处在于
其不需要独占一个对象。它可以和其他的boost::shared_ptr类型的智能指针共享所有权。
这样当引用对象的最后一个智能指针销毁后，对象才会被释放
*/
void test(){
	std::vector<boost::shared_ptr<int>> v;
	v.push_back(boost::shared_ptr<int>(new int(1)));
	v.push_back(boost::shared_ptr<int>(new int(2)));
}
/*
定义了两个共享指针i1和i2,它们都引用到同一个int类型的对象。i1通过new操作符返回
的地址显示的初始化，i2通过i1拷贝构造而来。i1接着调用reset()。它所包含的整数的地址被重新初始化。不过它之前所包含的对象并没有被
四方。因为i2仍然引用着它。智能指针boost::shared_ptr记录了有多少个共享指针在引用同一个对象，只有在最后一个共享指针销毁时才会释放这个对象。
需要注意的是共享指针默认是使用delete操作符来销毁所包含的对象。然而通过什么方法来进行销毁是可以进行指定的如
*/
void testsharedptrdeleter() {
	boost::shared_ptr<void> h(OpenProcess(PROCESS_SET_INFORMATION, FALSE, GetCurrentProcessId()), CloseHandle);
	SetPriorityClass(h.get(), HIGH_PRIORITY_CLASS);
}