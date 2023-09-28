#include "../../include/Command/command.h"

namespace _PROJECT_NAME_ 
{
	namespace _MOD_NAME_ 
	{
		std::string SOURCE_FILE_NAME = _SOURCE_FILE_NAME_;
		int IO_BUFF_SIZE = _IO_BUFF_SIZE_;
		int MATRIX_NUM = IO_BUFF_SIZE / _BP_COUNT_UNIT_;

		void getParameters(int argc, char* argv[])
		{
			for (int i = 1; i < argc; i++)
			{
				std::string cmd = argv[i];
				if (cmd == "-b")
				{
					IO_BUFF_SIZE = std::stoi((std::string)argv[++i]);
				}
				else if (cmd == "-s")
				{
					SOURCE_FILE_NAME = (std::string)argv[++i];
				}
			}

			std::cout << "Buffer size: " << IO_BUFF_SIZE << "\n";
			std::cout << "File Source: " << SOURCE_FILE_NAME << "\n";

		}

		int getBuildParameters(char cmd)
		{
			switch (cmd)
			{
			case 'a'://生成30bp
				std::cout << "Creat 30bp tab\n";
				return 30;
			case 'b'://生成30 60bp表
				return 60;
			case 'm'://合并现有表
				return 1;

			case 'q':
				return 0;
			default:
				return -1;
				break;
			}
		}

		std::string getSOURCE_FILE_NAME() {
			return SOURCE_FILE_NAME;
		};

		int getIO_BUFF_SIZE() {
			return IO_BUFF_SIZE;
		};

		int getMATRIX_NUM() {
			return MATRIX_NUM;
		}
	}
}