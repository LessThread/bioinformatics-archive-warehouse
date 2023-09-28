#pragma comment(lib,"./memBplus_O1.lib")

namespace PROJECT_NAME
{
    namespace LIB
    {
        __declspec(dllimport) int testFunc();
    }
}

int main(){
    PROJECT_NAME::LIB::testFunc();
}