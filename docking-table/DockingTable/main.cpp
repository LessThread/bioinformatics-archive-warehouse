#include "include/Settings.h"

#include <vector>
#include <string>

#include "include/Kmeans/Kmeans.h"
#include "include/IO/IO.h"
#include "include/Exception/Exception.h"
#include "include/QuickSort/QuickSort.h"
#include "include/CreateTab/CreateTab.h"



//全局变量

std::mutex tlock;

namespace _PROJECT_NAME_ {
	std::vector<_PROJECT_NAME_::KMEANS::Kmeans*>kmeans_vector;
	std::multimap<float, std::string>back_map;
}

int main(int argc, char* argv[])
{
	_PROJECT_NAME_::COMMAND_LINE::getParameters(argc, argv);

	try {
	_PROJECT_NAME_::IO::readFile();
	}
	catch (int code) {
		_PROJECT_NAME_::EXCEPTION::outputErrorCode(code);
	}
	catch (...) {
		_PROJECT_NAME_::EXCEPTION::outputUnkownErr("readFile");
	}

	_PROJECT_NAME_::QUICK_SORT::generateSortUnit();
	
	std::cout <<"Initialization complete, size: " << _PROJECT_NAME_::kmeans_vector.size()<<"\nPlease specify the build command: \n";

	while (true)
	{
		std::cout << "wait for commond\n";
		char cmd;
		std::cin >> cmd;
		int code = _PROJECT_NAME_::COMMAND_LINE::getBuildParameters(cmd);
		if (code == 0)
		{
			std::cout << "vbr0.1 2022.11\n--EXIT--";
			exit(0);
		}
		else if ( code == -1)
		{
			std::cout << "Unknown parameter, type 'q' to exit.\nPlease specify the build command: \n";
		}
		else
		{
			_PROJECT_NAME_::CREATE_TAB::createTab(code);
		}
	}
	

}

