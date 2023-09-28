#pragma once
#include "./settings.h"
#include <iostream>
#include <vector>
#include <fstream>
#include "./SequenceUnit.h"

#undef _MOD_NAME_
#define _MOD_NAME_ IO

namespace _PROJECT_NAME_ 
{
    namespace _MOD_NAME_
    {
        void readFile(std::string in_file_name);
        void writeFile(std::string out_file_name);
        void readFileFormOld(std::string FILE_NAME);//此函数用于从旧代码中直接调用以生成变化率
        void writeFileToOld(std::string FILE_NAME);
    }
    
}