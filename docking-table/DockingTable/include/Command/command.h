#pragma once
#include <string>
#include <iostream>
#include "../Settings.h"

#undef _MOD_NAME_
#define _MOD_NAME_ COMMAND_LINE

//Ԥ�������в�������
namespace _PROJECT_NAME_ {
	namespace _MOD_NAME_ {
		void getParameters(int argc,char* argv[]);
		std::string getSOURCE_FILE_NAME();
		int getBuildParameters(char);
		int getIO_BUFF_SIZE();
		int getMATRIX_NUM();
	}
}