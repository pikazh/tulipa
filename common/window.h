#pragma once

#include <functional>

#include "comdef.h"

BEGIN_NS_TULIPA

class Window {
public:
	virtual void SetIdleHandler(std::function<void(void)> handler) = 0;
	virtual void SetDrawHandler(std::function<void(void)> handler) = 0;
	virtual void SetCreateHandler(std::function<void(void)> handler) = 0;

	virtual void OnIdle() = 0;
};

END_NS_TULIPA
