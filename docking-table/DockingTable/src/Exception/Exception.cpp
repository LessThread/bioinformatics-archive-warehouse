#include "../../include/Exception/Exception.h"

namespace _PROJECT_NAME_
{
	namespace _MOD_NAME_
	{
		void outputErrorCode(int code)
		{
			std::cout << "Error: " << code<<"    ";
			switch (code)
			{
			case 1:
				std::cout << "File not found.\n"; break;
			case 2:
				std::cout << "RessultFile create unsuccessfully.\n"; break;
			default:
				std::cout << "Undefined error code.\n"; break;
			}

			exit(-1);
		}

		void outputUnkownErr(std::string mod_name)
		{
			std::cout << "unknown error: " << mod_name << "\n";
		}
	}
}