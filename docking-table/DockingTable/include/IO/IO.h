#pragma once

#include "../Settings.h"
#include "../Kmeans/Kmeans.h"
#include "../../include/Command/command.h"
#include "../../include/Exception/Exception.h"

#include <iostream>
#include <fstream>
#include <map>
#include <string>

#undef _MOD_NAME_
#define _MOD_NAME_ IO

namespace _PROJECT_NAME_ {
	namespace _MOD_NAME_ {
		//���뺯��������ȡ�������У�Ĭ����settings.h��
		int readFile(std::string FileName = (const std::string)_PROJECT_NAME_::COMMAND_LINE::getSOURCE_FILE_NAME());
		int outputResult(std::multimap<std::string, std::string>*,int);
		int outputResult(std::vector<std::vector<std::string*>*>* main_res_vector,
			std::vector< std::vector<std::string*>*>* main_res_vector_u, int code );
	}
}