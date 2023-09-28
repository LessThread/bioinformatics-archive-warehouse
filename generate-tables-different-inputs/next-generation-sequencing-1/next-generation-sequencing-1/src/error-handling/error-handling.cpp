#include "../../include/error-handling/error-handling.h"

namespace PROJECT_NAME
{
	namespace MOD_NAME
	{
		void outputErrorCode(std::string error_header, int error_code)
		{
			std::cout << error_header << error_code << std::endl;
			debugger;
		}
	}
}