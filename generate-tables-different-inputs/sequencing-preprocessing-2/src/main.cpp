#include "./settings.h"
#include "./IO/IO.h"
#include "./class/sequencing/sequencing.h"
#include "./exception-handling/exception-handling.h"

namespace PROJECT_NAME
{
    std::vector<std::vector<::PROJECT_NAME::SQE::sequencing>> SeqVector;
    std::map<::std::string,LONG> HashMap;
    std::mutex lock;
    std::mutex output_lock;
    int G_TASK_START = 1;
    int G_TASK_END = 10000000;
}
    

int main()
{
    PROJECT_NAME::IO::readFile();
    PROJECT_NAME::EXCEPTION::printOK("File complete read in");
    PROJECT_NAME::IO::outputFile();
    PROJECT_NAME::EXCEPTION::printOK("File completion output");
    debugger;
}