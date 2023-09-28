#include "../../include/Bplus/Bplus.h"

namespace PROJECT_NAME
{
	namespace MOD_NAME
	{
		PROJECT_NAME::LIB::MEM_BPLUS::BPlusTree* initBPlusTree()
		{
			PROJECT_NAME::LIB::MEM_BPLUS::BPlusTree T;
			T = PROJECT_NAME::LIB::MEM_BPLUS::Initialize();

			//���ļ������ļ�����д��B+����
			const std::string FilePath = BPLUES_INIT_FILE_NAME;
			PROJECT_NAME::LIB::MEM_BPLUS::readFile(FilePath, T);

			//������ɳ�ʼ����B+��ָ��
			return &T;
		}

	}
}