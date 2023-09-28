#pragma once
#include "../Settings.h"
#include "../Kmeans/Kmeans.h"
#include "../IO/IO.h"
#include <mutex>
#include <thread>
#include <map>
#include <vector>
#include <string>

#undef _MOD_NAME_
#define _MOD_NAME_	CREATE_TAB
#undef pair(A,B)
#define pair(A,B) std::pair<std::string, std::string>(A,B)


namespace _PROJECT_NAME_
{
	namespace _MOD_NAME_
	{
		int createTab(int);
	}
}

