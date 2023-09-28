#pragma once
#include "../../settings.h"

#undef MOD_NAME
#define MOD_NAME SQE

namespace PROJECT_NAME::MOD_NAME
{
    class sequencing
    {
    private:
    public:
        LONG ClassIndex;
        LONG index;
        std::string seq;
        std::string weight;
        std::string hash;

        sequencing(LONG index,std::string seq,std::string weight);
        sequencing(char index[],char seq[],char weight[]);
        sequencing(LONG index,char seq[],char weight[]);
        sequencing(char index[],char seq[]);
        ~sequencing();
        void setHash(std::string hash);
        void generateHash();
    };
}