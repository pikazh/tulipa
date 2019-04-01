#pragma once

#include <vector>
#include <functional>
#include <memory>
#include <tuple>

#include "commondef.h"

BEGIN_NS_TULIPA

class AtExitManager
{
protected:
	DISALLOW_COPY_AND_ASSIGN(AtExitManager);

public:
	AtExitManager() = default;
	~AtExitManager();

	template<typename FUNC, typename CLS, typename ...Params>
	void RegisterCallback(FUNC&& func, std::weak_ptr<CLS> &obj, Params&&... params)
	{
		auto cb = [f=std::forward<FUNC>(func), obj, args = std::make_tuple(std::forward<Params>(params)...)]() {
			auto sp_obj = obj.lock();
			if (!!sp_obj)
			{
				std::apply([&f, &sp_obj](auto&&... a) {
					(sp_obj.get()->*f)(a...);
				}, std::move(args));
			}
		};

		callbacks_.push_back(cb);
	}

	template<typename FUNC, typename CLS, typename ...Params>
	void RegisterCallback(FUNC&& func, std::shared_ptr<CLS> &obj, Params&&... params)
	{
		std::weak_ptr<CLS> ptr = obj;
		auto cb = [f = std::forward<FUNC>(func), ptr, args = std::make_tuple(std::forward<Params>(params)...)]() {
			auto sp_obj = ptr.lock();
			if (!!sp_obj)
			{
				std::apply([&f, &sp_obj](auto&&... a) {
					(sp_obj.get()->*f)(a...);
				}, std::move(args));
			}
		};

		callbacks_.push_back(cb);
	}

	template<typename FUNC, typename ...Params>
	void RegisterCallback(FUNC&& func, Params&&... params)
	{
		auto cb = std::bind(std::forward<FUNC>(func), std::forward<Params>(params)...);
		callbacks_.push_back(cb);
	}

private:
	std::vector<std::function<void(void)>> callbacks_;
};

END_NS_TULIPA