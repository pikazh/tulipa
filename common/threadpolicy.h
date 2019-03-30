#pragma once

#include <thread>
#include <mutex>
#include <cassert>

#include "commondef.h"

BEGIN_NS_TULIPA

class ThreadPolicyOperation :
	public std::enable_shared_from_this<ThreadPolicyOperation> {
public:
	virtual ~ThreadPolicyOperation() = default;
};

class SingleThreadCheckOperation :
	public ThreadPolicyOperation,
	public std::enable_shared_from_this<SingleThreadCheckOperation>{
public:
	SingleThreadCheckOperation(std::thread::id id) {
		if (std::this_thread::get_id() != id) {
			//Attention!!!The object is running on another thread that is different from the one
			//on which the object was created.
			//assert(0);
#ifdef _WIN32
			::DebugBreak();
#endif
		}
		
	}
};

class MulitThreadResLockOperation :
	public ThreadPolicyOperation,
	public std::enable_shared_from_this<MulitThreadResLockOperation> {
public:
	MulitThreadResLockOperation(std::mutex &mutex) :
		mutex_(mutex) {
		mutex_.lock();
	}
	virtual ~MulitThreadResLockOperation() {
		mutex_.unlock();
	}

private:
	std::mutex &mutex_;
};

class ThreadPolicyBase {
public:
	virtual std::shared_ptr<ThreadPolicyOperation> OnPolicyInit() { return std::shared_ptr<ThreadPolicyOperation>(); }
	virtual std::shared_ptr<ThreadPolicyOperation> OnFunctionCall() { return std::shared_ptr<ThreadPolicyOperation>(); }
};

class SingleThreadCheckPolicy
	: public ThreadPolicyBase {
public:
	SingleThreadCheckPolicy()
#ifdef _DEBUG
		: id_(std::this_thread::get_id()) 
#endif
	{

	}
	virtual std::shared_ptr<ThreadPolicyOperation> OnFunctionCall() override;
protected:
	DISALLOW_COPY_AND_ASSIGN(SingleThreadCheckPolicy);
private:
#ifdef _DEBUG
	std::thread::id id_;
#endif
};

class MultiThreadResLockPolicy
	: public ThreadPolicyBase {
public:
	MultiThreadResLockPolicy() = default;
	virtual std::shared_ptr<ThreadPolicyOperation> OnFunctionCall() override;
protected:
	DISALLOW_COPY_AND_ASSIGN(MultiThreadResLockPolicy);
private:
	std::mutex l_;
};

END_NS_TULIPA
