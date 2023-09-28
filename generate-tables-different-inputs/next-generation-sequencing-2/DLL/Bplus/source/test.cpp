#pragma comment(lib,"./memBplus.lib")
#include <iostream>

namespace NEXT_GENERATION_SEQUENCING{
    namespace LIB{
        namespace MEM_BPLUS{
            struct __declspec(dllexport) BPlusNode ;
            __declspec(dllimport) BPlusNode* Initialize();
        }
    }
}

int main()
{
    NEXT_GENERATION_SEQUENCING::LIB::MEM_BPLUS::Initialize();
    printf("yes");
}