#include "../../include/EigenMatrix/EigenMatrix.h"

namespace _PROJECT_NAME_ {
    namespace _MOD_NAME_ {
        void initEigMatrix(std::string in_file_name, std::string out_file_name)
        {
            try {
                std::cout << in_file_name << "\n" << out_file_name <<"\n";
                callEigenMatrix( in_file_name.c_str(), out_file_name.c_str());
            }
            catch (...) {

            }
        }
    }
}

