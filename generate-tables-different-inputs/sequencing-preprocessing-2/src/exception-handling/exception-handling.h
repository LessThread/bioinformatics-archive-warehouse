#pragma once
#include "../settings.h"

#undef MOD_NAME
#define MOD_NAME EXCEPTION

namespace PROJECT_NAME::MOD_NAME
{
    void printWarnning(std::string information);
    void printError(std::string information);
    void printOK(std::string information);
}