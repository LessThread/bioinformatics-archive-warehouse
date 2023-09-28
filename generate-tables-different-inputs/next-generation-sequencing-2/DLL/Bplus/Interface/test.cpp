#pragma comment(lib,"./Bplus.dll")

#include"../main-settings.h"
#undef MOD_NAME
#define MOD_NAME BPLUS

namespace PROJECT_NAME
{
	namespace LIB
	{
        namespace MEM_BPLUS
        {
            exturn __declspec(dllimport)PROJECT_NAME::LIB::MEM_BPLUS::BPlusTree*;
            __declspec(dllimport)PROJECT_NAME::LIB::MEM_BPLUS::BPlusTree* initBPlusTree();
        }
        
    }
}

int main(){
    PROJECT_NAME::LIB::MEM_BPLUS::BPlusTree* initBPlusTree()
}