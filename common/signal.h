#pragma once

#include <memory>
#include <map>
#include <functional>

#include "comdef.h"
#include "threadpolicy.h"

BEGIN_NS_TULIPA

enum class ThreadPolicyType
{
	only_in_single_thread,
	support_multi_thread
};

template<typename ...Params>
class Signal
{
public:

	Signal(ThreadPolicyType type = ThreadPolicyType::only_in_single_thread)
		:id_(0),type_(type) {
		CreateThreadPolicy();
	}

	virtual ~Signal() {
		DisconnectAll();
	}

	uint32_t Connect(std::function<void(Params...)> func)
	{
		auto thread_policy = thread_policy_ptr_->OnFunctionCall();

		callbacks_[id_] = func;

		return id_++;
	}

	template<typename CLS>
	uint32_t Connect(void(CLS::*fptr)(Params...), CLS &obj)
	{
		auto thread_policy = thread_policy_ptr_->OnFunctionCall();

		callbacks_[id_] = ([&obj, fptr](Params... p) {
			(obj.*fptr)(std::forward<Params>(p)...);
		});

		return id_++;
	}

	template<typename CLS>
	uint32_t Connect(void(CLS::*fptr)(Params...), std::weak_ptr<CLS> &obj)
	{
		auto thread_policy = thread_policy_ptr_->OnFunctionCall();

		callbacks_[id_] = ([obj, fptr](Params... p) {
			std::shared_ptr<CLS> sp_obj = obj.lock();
			if (!!sp_obj) {
				(sp_obj.get()->*fptr)(std::forward<Params>(p)...);
			}
		});

		return id_++;
	}

	template<typename CLS>
	uint32_t Connect(void(CLS::*fptr)(Params...), std::shared_ptr<CLS> &obj)
	{
		auto thread_policy = thread_policy_ptr_->OnFunctionCall();
		
		std::weak_ptr<CLS> ptr = obj;
		callbacks_[id_] = ([ptr, fptr](Params... p) {
			std::shared_ptr<CLS> sp_obj = ptr.lock();
			if (!!sp_obj) {
				(sp_obj.get()->*fptr)(std::forward<Params>(p)...);
			}
		});

		return id_++;
	}

	void DisconnectAll() {
		auto thread_policy = thread_policy_ptr_->OnFunctionCall();

		id_ = 0;
		callbacks_.clear();
	}

	void Disconnect(uint32_t id) {
		auto thread_policy = thread_policy_ptr_->OnFunctionCall();

		auto it = callbacks_.find(id);
		if(it != callbacks_.end())
			callbacks_.erase(it);
	}

	void Emit(Params... p) {
		auto thread_policy = thread_policy_ptr_->OnFunctionCall();

		for (auto it = callbacks_.begin(); it != callbacks_.end(); ++it) {
			it->second(std::forward<Params>(p)...);
		}
	}


protected:
	DISALLOW_COPY_AND_ASSIGN(Signal);

	std::unique_ptr<ThreadPolicyBase>& thread_policy_ptr() {
		return thread_policy_ptr_;
	}

	void CreateThreadPolicy() {
		switch (type_) {
		case ThreadPolicyType::only_in_single_thread:
			thread_policy_ptr() = std::move(std::make_unique<SingleThreadCheckPolicy>());
			break;
		case ThreadPolicyType::support_multi_thread:
			thread_policy_ptr() = std::move(std::make_unique<MultiThreadResLockPolicy>());
			break;
		default:
			assert(false);
		}
	}

private:
	ThreadPolicyType type_;
	uint32_t id_;
	std::map<uint32_t, std::function<void(Params...)>> callbacks_;
	std::unique_ptr<ThreadPolicyBase> thread_policy_ptr_;
};

END_NS_TULIPA
