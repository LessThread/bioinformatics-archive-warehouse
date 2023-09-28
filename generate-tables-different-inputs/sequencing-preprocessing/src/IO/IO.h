#pragma once
#include "../settings.h"
#include "../exception-handling/exception-handling.h"
#include "../class/sequencing/sequencing.h"
#include <fstream>
#include <cstring>
#include <thread>

#undef MOD_NAME
#define MOD_NAME IO

namespace PROJECT_NAME::MOD_NAME
{
    void readFile(std::string FilePath = INPUT_FILE_PATH);
    void outputFile(std::string QNFilePath = OUTPUT_QN_FILE_PATH,std::string CQFilePath = OUTPUT_CQ_FILE_PATH);
}

namespace PROJECT_NAME
{
    extern std::vector<std::vector<::PROJECT_NAME::SQE::sequencing>> SeqVector;
    extern std::map<::std::string,LONG> HashMap;
    extern std::mutex lock;
}
