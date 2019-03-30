#pragma once

#include "common/commondef.h"
#include "common/gui.h"
#include "mswindow.h"

BEGIN_NS_TULIPA

class MSGUI:
	public GUI
{
public:
	MSGUI();
	virtual ~MSGUI();

	virtual uint32_t CreateWin() override;
	virtual uint32_t Run() override;
	virtual Window* main_window() override;

protected:
	DISALLOW_COPY_AND_ASSIGN(MSGUI);

private:
	MsWindow *win_;
};

END_NS_TULIPA
