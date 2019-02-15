#include "easy_app.h"

BEGIN_NS_TULIPA

Easy_App* Easy_App::global_instance()
{
	return global_instance_;
}

Easy_App::Easy_App():
	gui_(nullptr)
{
	global_instance_ = this;
}

Easy_App::~Easy_App()
{
	if (gui_) {
		delete gui_;
		gui_ = nullptr;
	}
}

uint32_t tulipa::Easy_App::Run()
{
	return gui_->Run();
}

bool Easy_App::Init()
{
	gui_ = CreateGUI();
	if (gui_ == nullptr)
		return false;

	auto main_wnd = gui_->main_window();
	main_wnd->SetCreateHandler(std::bind(&Easy_App::OnCreate, this));
	main_wnd->SetIdleHandler(std::bind(&Easy_App::OnIdle, this));
	main_wnd->SetDrawHandler(std::bind(&Easy_App::OnDraw, this));

	if (0 != gui_->CreateWin())
		return false;

	return true;
}

Easy_App* Easy_App::global_instance_ = nullptr;

END_NS_TULIPA
