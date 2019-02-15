#pragma once

#include <stdint.h>

#include "window.h"
#include "comdef.h"

BEGIN_NS_TULIPA

class GUI
{
public:
	virtual uint32_t CreateWin() = 0;
	virtual uint32_t Run() = 0;
	virtual Window* main_window() = 0;
};

END_NS_TULIPA
