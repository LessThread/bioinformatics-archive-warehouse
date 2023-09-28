#pragma once
#include "../Settings.h"
#include "../Kmeans/Kmeans.h"

#include <string>
#include <map>
#include <vector>
#include <iostream>

#undef _MOD_NAME_
#define _MOD_NAME_	QUICK_SORT
#define pair(A,B) std::pair<float, std::string>(A,B)

namespace _PROJECT_NAME_
{
	namespace _MOD_NAME_
	{
		class sort_unit
		{
		public:
			std::string* ID;
			float* value;
			sort_unit();
			~sort_unit();

		private:

		};

		int generateSortUnit();
	}
}