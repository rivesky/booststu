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
ָ������������ͨ����ͨ��vecotr���д�ţ����ǿ��ǵ����������ü����ĸı䣬����е���������Ե�ָ���������д�š�
*/
/*
����ʽָ�룬�����ϣ�����ʽָ��Ĺ�����ʽ�͹���ָ����ȫһ����boost::shared_ptr���ڲ���¼�����õ�ĳ������Ĺ���ָ������������ǶԽ���
ʽָ����˵������Ա�͵��Լ�������¼�����ڿ�ܶ�����˵����ر����ã���Ϊ���Ǽ�¼������i���õĴ�����
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
��ָ��ֻ����Ϲ���ָ��һ��ʹ��ʱ�������塣
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
boost::weak_ptr�ض�����ͨ��boost::shared_ptr����ʼ���ġ�һ����ʼ������������ֻ�ṩһ�����õķ�����lock().
�÷������ص�boost::shared_ptr��������ʼ����ָ��Ĺ���ָ�빲������Ȩ������������ָ�벻�����κζ��󡣷��صĹ���ָ��Ҳ���ǿյġ�
ע�⵱������Ҫһ���ɹ���ָ���ƹ���������󣬺����Ϳ���ʹ�øö����������ָ�븴λ�ˡ����㺯�����ܵõ�һ������ָ�룬����Ҳ�������ˡ�
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
�����������Ϊ�����빲��ָ�룬�ؼ���ͬ���ڹ�������������ʱ��Ĭ��ʹ��delte[]���������ͷ������Ķ�����Ϊ���������ֻ�������������
�����������ͨ����̬���������ĵ�ַ����ʼ��
*/
void sharedsarrayptr() {
	boost::shared_array<int> i1(new int[2]);
	boost::shared_array<int> i2(i1);
	i1[0] = 1;
	std::cout << i2[0] << std::endl;
}


/*
��������ָ��boost::shared_ptr������������boost::scoped_ptr.�ؼ���֮ͬ������
�䲻��Ҫ��ռһ�����������Ժ�������boost::shared_ptr���͵�����ָ�빲������Ȩ��
���������ö�������һ������ָ�����ٺ󣬶���Żᱻ�ͷ�
*/
void test(){
	std::vector<boost::shared_ptr<int>> v;
	v.push_back(boost::shared_ptr<int>(new int(1)));
	v.push_back(boost::shared_ptr<int>(new int(2)));
}
/*
��������������ָ��i1��i2,���Ƕ����õ�ͬһ��int���͵Ķ���i1ͨ��new����������
�ĵ�ַ��ʾ�ĳ�ʼ����i2ͨ��i1�������������i1���ŵ���reset()�����������������ĵ�ַ�����³�ʼ����������֮ǰ�������Ķ���û�б�
�ķ�����Ϊi2��Ȼ��������������ָ��boost::shared_ptr��¼���ж��ٸ�����ָ��������ͬһ������ֻ�������һ������ָ������ʱ�Ż��ͷ��������
��Ҫע����ǹ���ָ��Ĭ����ʹ��delete�������������������Ķ���Ȼ��ͨ��ʲô���������������ǿ��Խ���ָ������
*/
void testsharedptrdeleter() {
	boost::shared_ptr<void> h(OpenProcess(PROCESS_SET_INFORMATION, FALSE, GetCurrentProcessId()), CloseHandle);
	SetPriorityClass(h.get(), HIGH_PRIORITY_CLASS);
}