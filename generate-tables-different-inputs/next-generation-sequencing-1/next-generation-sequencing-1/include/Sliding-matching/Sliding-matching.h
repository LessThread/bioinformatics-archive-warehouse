#pragma once
#include "../../include/include.h"
#include "../../main-settings.h"
#include "./settings.h"
#include "../error-handling/error-handling.h"
#include "../../lib/ID-washing/ID-wash.cpp"

#undef MOD_NAME
#define MOD_NAME SLIDING_MATCHING

namespace PROJECT_NAME 
{
	namespace MOD_NAME
	{
		int matchSequence(std::string* MainSequence,std::string* SlidingSequence);
	}
}