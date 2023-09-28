#pragma once
#include "../source/memBplus.h"

#undef MOD_NAME
#define MOD_NAME BPLUS

namespace PROJECT_NAME
{
	namespace MOD_NAME 
	{
		__declspec(dllexport) PROJECT_NAME::LIB::MEM_BPLUS::BPlusTree* initBPlusTree();
	}
}