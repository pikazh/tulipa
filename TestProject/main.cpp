#include "common/easy_app.h"
#include "common/msgui.h"

BEGIN_NS_TULIPA
class MyApp :
	public Easy_App {

protected:
	virtual GUI* CreateGUI() override
	{
		return new MSGUI();
	}
};
END_NS_TULIPA

int WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd ) {
	
	tulipa::MyApp app;
	if (app.Init())
		return app.Run();
	else
		return 1;
}