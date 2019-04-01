#include <thread>
#include <iostream>
#include <string>
#include <windows.h>
#include <assert.h>

#include "common/signal.h"

void TestFunc1(int *i,const std::string &str)
{
	std::cout << "calling TestFunc1 on thread " << std::this_thread::get_id() << std::endl;
	std::cout << "i = " << *i << ", str = " << str << std::endl;
	*i = (*i)+1;
}

class TestClass
{
public:
	static void StaticFuncTest1()
	{
		std::cout << "calling TestClass::StaticFuncTest1 " << std::endl;
	}

	static void StaticFuncTest2(int i)
	{
		std::cout << "calling TestClass::StaticFuncTest2 with i = " << i << std::endl;
	}

	void FuncTest1(int &&i)
	{
		std::cout << "calling TestClass::FuncTest1, i is " << i++ << std::endl;
	}

};

int main()
{
	std::shared_ptr<tulipa::Signal<int*, const std::string&>> st_signal_obj1 = std::make_shared<tulipa::Signal<int*, const std::string&>>();
	std::shared_ptr<tulipa::Signal<int*, const std::string&>> mt_signal_obj1 = std::make_shared<tulipa::Signal<int*, const std::string&>>(tulipa::ThreadPolicyType::support_multi_thread);
	std::shared_ptr<tulipa::Signal<HWND, LPCWSTR, LPCWSTR, UINT>> mt_signal_obj2 = std::make_shared<tulipa::Signal<HWND, LPCWSTR, LPCWSTR, UINT>>(tulipa::ThreadPolicyType::support_multi_thread);
	std::shared_ptr<tulipa::Signal<>> mt_signal_obj3 = std::make_shared<tulipa::Signal<>>(tulipa::ThreadPolicyType::support_multi_thread);
	std::shared_ptr<tulipa::Signal<int&&>> mt_signal_obj4 = std::make_shared<tulipa::Signal<int&&>>(tulipa::ThreadPolicyType::support_multi_thread);
	
	st_signal_obj1->Connect(TestFunc1);

	mt_signal_obj1->Connect(TestFunc1);

	mt_signal_obj2->Connect(::MessageBoxW);

	mt_signal_obj3->Connect(TestClass::StaticFuncTest1);

	uint32_t receiver_id = mt_signal_obj3->Connect(std::bind(TestClass::StaticFuncTest2, 2));

	std::shared_ptr<TestClass> testObj = std::make_shared<TestClass>();
	TestClass testObj2;
	mt_signal_obj4->Connect(&TestClass::FuncTest1, testObj);
	mt_signal_obj4->Connect(&TestClass::FuncTest1, testObj2);

	//test with lambda
	mt_signal_obj4->Connect([](int &&i) {
		std::cout << "calling lambda, i is " << i << std::endl;
	});

	int i = 32;
	st_signal_obj1->Emit(&i, "hello");

	std::thread new_thread([st_signal_obj1, mt_signal_obj1, mt_signal_obj2, mt_signal_obj3, mt_signal_obj4, receiver_id]()
	{
		int i = 323;
		int j = i;
		mt_signal_obj1->Emit(&i, "world");

		mt_signal_obj2->Emit(nullptr, L"Hello world", L"title", MB_OK);

		mt_signal_obj3->Emit();
		mt_signal_obj3->Disconnect(receiver_id);
		mt_signal_obj3->Emit();

		mt_signal_obj4->Emit(std::move(i));

		//i shall have been changed
		assert(i != j);

		//this call will throw an exception as the signal obj is only single-thread
		st_signal_obj1->Emit(&i, "exception");
	}
	);

	new_thread.join();
	return 0;
}