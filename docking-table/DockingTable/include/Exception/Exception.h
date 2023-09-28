#pragma once
#include "../../include/Settings.h"

#include <iostream>

#undef _MOD_NAME_
#define _MOD_NAME_ EXCEPTION

namespace _PROJECT_NAME_
{
	namespace _MOD_NAME_
	{
		void outputErrorCode(int code);
		void outputUnkownErr(std::string mod_name);
	}
}
