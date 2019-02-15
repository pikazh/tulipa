#pragma once

#include <stdint.h>
#include <memory>
#include <map>
#include <functional>

#include "comdef.h"
#include "threadpolicy.h"

BEGIN_NS_TULIPA

template<typename ...Params>
class Signal
{
public:

	enum class ThreadPolicyType
	{
		only_in_single_thread,
		support_multi_thread
	};

	Signal(ThreadPolicyType type = ThreadPolicyType::only_in_single_thread)
		:id_(0),type_(type) {
		CreateThreadPolicy();
	}

	virtual ~Signal() {
		DisconnectAll();
	}

	template<typename CLS, typename FUNC>
	uint32_t Connect(std::weak_ptr<CLS> &obj, FUNC &&func)
	{
		auto thread_policy = thread_policy_ptr_->OnFunctionCall();

		//Generalized lambda capture in C++14
		callbacks_[id_] = ([obj, f{ std::forward<FUNC>(func) }](Params&&... p) {
			std::shared_ptr<CLS> sp_obj = obj.lock();
			if (!!sp_obj) {
				(sp_obj.get()->*f)(std::forward<Params>(p)...);
			}
		});

		return id_++;
	}

	template<typename CLS, typename FUNC>
	uint32_t Connect(std::weak_ptr<CLS> &&obj, FUNC &&func)
	{
		auto thread_policy = thread_policy_ptr_->OnFunctionCall();

		//Generalized lambda capture in C++14
		callbacks_[id_] = ([o{ std::forward<std::weak_ptr<CLS>>(obj) }, f{ std::forward<FUNC>(func) }](Params&&... p) {
			std::shared_ptr<CLS> sp_obj = o.lock();
			if (!!sp_obj) {
				(sp_obj.get()->*f)(std::forward<Params>(p)...);
			}
		});

		return id_++;
	}

	uint32_t Connect(std::function<void(Params...)> func)
	{
		auto thread_policy = thread_policy_ptr_->OnFunctionCall();

		callbacks_[id_] = func;

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

	void Emit(Params&&... p) {
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
