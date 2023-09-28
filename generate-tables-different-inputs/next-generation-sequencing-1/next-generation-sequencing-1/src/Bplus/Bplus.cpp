#include "../../include/Bplus/Bplus.h"

namespace PROJECT_NAME
{
	namespace MOD_NAME
	{
		PROJECT_NAME::LIB::MEM_BPLUS::BPlusTree* initBPlusTree()
		{
			PROJECT_NAME::LIB::MEM_BPLUS::BPlusTree T;
			T = PROJECT_NAME::LIB::MEM_BPLUS::Initialize();

			//读文件，将文件内容写入B+树中
			const std::string FilePath = BPLUES_INIT_FILE_NAME;
			PROJECT_NAME::LIB::MEM_BPLUS::readFile(FilePath, T);

			//返回完成初始化的B+树指针
			return &T;
		}

	}
}