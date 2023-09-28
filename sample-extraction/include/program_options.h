#pragma once

#include <iostream>
#include <string>
#include <boost/program_options.hpp>

#undef MOD_NAME
#define MOD_NAME OP




namespace  bpo = boost::program_options;


namespace MOD_NAME
{
    constexpr int op_generate(){return 1;} 

    int b_po(int argc, char const *argv[]);
}