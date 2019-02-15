#pragma once

#include "comdef.h"
#include "gui.h"
#include "window.h"

BEGIN_NS_TULIPA

class Easy_App{
public:

	static Easy_App* global_instance();

	Easy_App();
	virtual ~Easy_App();

	GUI* gui() {
		return gui_;
	}
	
	virtual bool Init();
	virtual uint32_t Run();

protected:
	virtual GUI* CreateGUI() = 0;

	virtual void OnCreate() {}
	virtual void OnDraw() {}
	virtual void OnIdle() {}

private:
	GUI*	gui_;
	static Easy_App* global_instance_;
};

END_NS_TULIPA
