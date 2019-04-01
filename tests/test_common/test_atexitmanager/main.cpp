#include <iostream>
#include <string>
#include <thread>
#include <windows.h>
#include <assert.h>

#include "common/atexitmanager.h"

class A
{
public:
	A() = default;
	~A() = default;

	void test(int &i, int *j)
	{
		i++;
		*j = *j + 1;
	}

};

void test(int &i, int *j)
{
	i++;
	*j = *j + 1;
}

int main()
{
	tulipa::AtExitManager at_exit_mng_main;

	at_exit_mng_main.RegisterCallback(::MessageBoxW, nullptr, L"program will end after this msgbox.", nullptr, MB_OK);

	at_exit_mng_main.RegisterCallback([](int i, const std::string  &str) {
		std::cout << "exit callback1 is running, i=" << i << " str=" << str << std::endl;
	}, 3, "hello");

	int i = 4;
	int j = 23;
	int i_ori = i;
	int j_ori = j;

	int x = 23;
	int y = 3;
	int x_ori = x;
	int y_ori = y;
	
	std::shared_ptr<A> a_obj = std::make_shared<A>();

	{
		tulipa::AtExitManager at_exit_local;
		at_exit_local.RegisterCallback(test, std::ref(i), &j);
		at_exit_local.RegisterCallback(&A::test, a_obj, std::ref(x), &y);
		
	}
	//after invoking test, i, j, x and y shall have been chaged.
	assert(i != i_ori && j != j_ori && x != x_ori && y != y_ori);
	
	std::cout << "doing some work..." << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(400));

	std::cout << "program is going to exit..." << std::endl;
	return 0;
}