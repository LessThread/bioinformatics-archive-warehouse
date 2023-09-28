#pragma once
#include "dll.h"

namespace _PROJECT_NAME_ {
	extern std::vector<_PROJECT_NAME_::KMEANS::Kmeans*>kmeans_vector;
	extern std::multimap<float, std::string>back_map;
}

__declspec(dllexport)void dockTable(std::string path)
{
		_PROJECT_NAME_::IO::readFile(path);

		_PROJECT_NAME_::QUICK_SORT::generateSortUnit();

		std::cout << "Initialization complete, size: " << _PROJECT_NAME_::kmeans_vector.size() << "\nPlease specify the build command: \n";

		int code = 30;

		_PROJECT_NAME_::CREATE_TAB::createTab(code);



}