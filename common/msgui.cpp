#include "msgui.h"

BEGIN_NS_TULIPA

MSGUI::MSGUI()
{
	win_ = new MsWindow();
}

MSGUI::~MSGUI()
{
	if (win_ != nullptr) {
		delete win_;
		win_ = nullptr;
	}
}

uint32_t MSGUI::Run()
{
	MSG msg;
	for (;;)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
			{
				break;
			}

			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			win_->OnIdle();
		}
	}

	return (int)msg.wParam;
}

Window* MSGUI::main_window()
{
	return win_;
}

uint32_t MSGUI::CreateWin()
{
	if (!win_->Create())
		return 1;

	return 0;
}

END_NS_TULIPA
