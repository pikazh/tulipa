#pragma once

#include "common/comdef.h"
#include "common/window.h"
#include "common/signal.h"

BEGIN_NS_TULIPA

class MsWindow:
	public Window{
public:
	MsWindow();
	virtual ~MsWindow();

	virtual void SetIdleHandler(std::function<void(void)> handler) override;
	virtual void SetDrawHandler(std::function<void(void)> handler) override;
	virtual void SetCreateHandler(std::function<void(void)> handler) override;
	virtual void OnIdle() override;
	bool Create();

protected:
	DISALLOW_COPY_AND_ASSIGN(MsWindow);

	ATOM RegisterWindowClass();
	LRESULT ProcessMessage(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

private:	
	static LRESULT CALLBACK	WinProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	static ATOM wnd_class_;
	static const std::wstring wnd_class_name_;

	HWND hwnd_;
	std::unique_ptr<Signal<>> create_signal_;
	std::unique_ptr<Signal<>> idle_signal_;
	std::unique_ptr<Signal<>> draw_signal_;
};

END_NS_TULIPA
