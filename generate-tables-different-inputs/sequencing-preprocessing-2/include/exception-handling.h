#pragma once
#include "../settings.h"

#undef MOD_NAME
#define MOD_NAME EXCEPTION

namespace PROJECT_NAME::MOD_NAME
{
    int printWarnning(std::string information);
    int printError(std::string information);
}