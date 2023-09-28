#include "./exception-handling.h"

namespace PROJECT_NAME::MOD_NAME
{
    void printWarnning(std::string information)
    {
        printf("\033[0m\033[1;33m[Warnning]\033[0m%s\n",information.data());
    }
    void printError(std::string information)
    {
        printf("\033[0m\033[1;31m[Error]\033[0m%s\n",information.data());
    }
    void printOK(std::string information)
    {
        printf("\033[0m\033[1;32m[OK]\033[0m%s\n",information.data());
    }
}