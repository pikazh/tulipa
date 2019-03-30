#pragma once

#ifdef _WIN32
#include <windows.h>
#endif


#define BEGIN_NS_TULIPA		namespace tulipa{
#define END_NS_TULIPA		}

#define DISALLOW_COPY_AND_ASSIGN(CLS)	CLS(const CLS&) = delete;\
CLS operator=(const CLS&) = delete
