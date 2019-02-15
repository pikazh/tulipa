#include "threadpolicy.h"

BEGIN_NS_TULIPA

std::shared_ptr<ThreadPolicyOperation> SingleThreadCheckPolicy::OnFunctionCall()
{
#ifdef _DEBUG
	return std::dynamic_pointer_cast<ThreadPolicyOperation>(std::make_shared<SingleThreadCheckOperation>(id_));
#else
	return ThreadPolicyBase::OnFunctionCall();
#endif
}

std::shared_ptr<ThreadPolicyOperation> MultiThreadResLockPolicy::OnFunctionCall()
{
	return std::dynamic_pointer_cast<ThreadPolicyOperation>(std::make_shared<MulitThreadResLockOperation>(l_));
}

END_NS_TULIPA



