#pragma once
#include"../../include/PublicHeader.h"


/*此处的路径是相对主项目下的文件，不是相对本文件的*/
//#pragma comment(lib, "./lib/lib-EigenMatrix/DLL_TEST.lib")
#pragma comment(lib, "./lib/lib-EigenMatrix/EigenMatrix.lib")


#undef _MOD_NAME_
#define _MOD_NAME_ EIGEN_MATRIX


extern "C" _declspec(dllexport)void callEigenMatrix(
    const char* $in_file_name,
    const char* $out_file_name);


namespace _PROJECT_NAME_{
    namespace _MOD_NAME_ {
        void initEigMatrix(std::string in_file_name = IN_FILE_NAME, std::string out_file_name = OUT_FILE_NAME);
    }
}
