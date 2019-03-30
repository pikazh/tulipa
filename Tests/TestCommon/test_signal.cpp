#include <thread>
#include <iostream>
#include <string>
#include <windows.h>

#include "common/signal.h"


void TestFunc1(int *i, std::string &&str)
{
	std::cout << "calling TestFunc1 on thread " << std::this_thread::get_id() << std::endl;
	std::cout << "i = " << *i << ", str = " << str << std::endl;
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
		i = 433;
		std::cout << "calling TestClass::FuncTest1, i is changed to 433" << std::endl;
	}

};

int main()
{
	std::shared_ptr<tulipa::Signal<int*, std::string&&>> st_signal_obj1 = std::make_shared<tulipa::Signal<int*, std::string&&>>();
	std::shared_ptr<tulipa::Signal<int*, std::string&&>> mt_signal_obj1 = std::make_shared<tulipa::Signal<int*, std::string&&>>(tulipa::ThreadPolicyType::support_multi_thread);
	std::shared_ptr<tulipa::Signal<HWND, LPCWSTR, LPCWSTR, UINT>> mt_signal_obj2 = std::make_shared<tulipa::Signal<HWND, LPCWSTR, LPCWSTR, UINT>>(tulipa::ThreadPolicyType::support_multi_thread);
	std::shared_ptr<tulipa::Signal<>> mt_signal_obj3 = std::make_shared<tulipa::Signal<>>(tulipa::ThreadPolicyType::support_multi_thread);
	std::shared_ptr<tulipa::Signal<int&&>> mt_signal_obj4 = std::make_shared<tulipa::Signal<int&&>>(tulipa::ThreadPolicyType::support_multi_thread);
	
	st_signal_obj1->Connect(TestFunc1);
	mt_signal_obj1->Connect(TestFunc1);
	mt_signal_obj2->Connect(::MessageBoxW);

	mt_signal_obj3->Connect(TestClass::StaticFuncTest1);

	//test signal with multi receiver and with bound arguments
	uint32_t receiver_id = mt_signal_obj3->Connect(std::bind(TestClass::StaticFuncTest2, 2));

	std::shared_ptr<TestClass> testObj = std::make_shared<TestClass>();
	mt_signal_obj4->Connect<TestClass>(testObj, &TestClass::FuncTest1);

	std::this_thread::sleep_for(std::chrono::seconds(1));

	int i = 32;
	st_signal_obj1->Emit(&i, "hello");

	std::thread new_thread([st_signal_obj1, mt_signal_obj1, mt_signal_obj2, mt_signal_obj3, mt_signal_obj4, receiver_id]()
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));

		int i = 323;
		std::string str = "world";
		mt_signal_obj1->Emit(&i, std::move(str));

		mt_signal_obj2->Emit(nullptr, L"Hello world", L"title", MB_OK);

		mt_signal_obj3->Emit();
		mt_signal_obj3->Disconnect(receiver_id);
		mt_signal_obj3->Emit();

		mt_signal_obj4->Emit(std::move(i));
		//i should be changed
		std::cout << i;

		//this call will throw an exception as the signal obj is only single-thread
		st_signal_obj1->Emit(&i, "exception");
	}
	);

	new_thread.join();
	return 0;
}