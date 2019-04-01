#include "atexitmanager.h"

BEGIN_NS_TULIPA

AtExitManager::~AtExitManager()
{
	//invoke the callback functions in reverse order, acting like a stack
	for (auto it = callbacks_.rbegin(); it != callbacks_.rend(); ++it)
	{
		(*it)();
	}
}

END_NS_TULIPA


