#include "IO.h"

void FileRead_and_BreakUp()
{
	std::fstream test;
	test.open("quickSort.txt", std::ios::in);

	const unsigned num = 100;

	char txt[num]={0};
	test.getline(txt, sizeof(char) * num);
	puts("qS\n");
	//std::cout << txt <<std::endl;

	int k = 0;
	/*for (int i = 0; i < num; i++)
	{
		printf("%c:%d\n", txt[i], txt[i]);
	}*/
	for (int i = 0; i < num; i++)
	{
		if (txt[i] == ' '&&k==0)
		{
			std::cout << "\n"; k = 1;
		}
		else if (txt[i] == '\n')
		{
			std::cout << "err\n";
		}
		else
		{
			std::cout << txt[i]; k = 0;
		}
	}
	system("pause");

}


std::string getBaseSequence(char *str,unsigned id)
{
	//for (int i = 0; i < id; i++)std::cout << "                      ";
	//std::cout<< str << "\n";
	free(str);
	return "AGCTGAGCTAGCTAGCTGAGCTAGCTAGCTGAGCTAGCTAGCTGAGCTAGCT";
}


void writeFile(std::string res[],unsigned id,unsigned count)
{
	std::fstream temp;

	std::string FileName = "src\\";
	FileName += std::to_string(id);
	FileName += ".txt";

	std::cout << FileName <<std::endl;
	temp.open(FileName, std::ios::out | std::ios::app);
	if (temp.good())
	{
		for (int i = 0; i < id; i++)
		{
			temp << "        ";
		}

		for (int i = 0; i < count; i++)
		{
			temp << res[i];
		}

		temp << "\n";

		std::cout << "BaseSort_File opened\n";
	}
	else
	{
		std::cout << " err in writeFile\n";
		exit(0);
	}
	temp.close();
}

void mergeFile(std::string fileName[])
{

}

void mergeFile(unsigned uALL_LIN )
{
	system("pause");
	std::cout << uALL_LIN;

	char* ResourcePool = (char*)malloc(sizeof(char) * BUF_SIZE);
	std::string pool;
	std::string file = "src\\";

	for (int i = 0;i<uALL_LIN; i+= POOL_BUF)
	{
		for (int j = i; j < POOL_BUF; j++)
		{
			file += std::to_string(j);
			file += ".txt";
			
			std::cout << file<<std::endl;

			std::fstream read;
			read.open(file,std::ios::in);

			read.getline(ResourcePool, sizeof(char) * BUF_SIZE);
			pool += ResourcePool;
			pool += '\n';
			read.close();//看要不要加换行符
			file = "src\\";
			
		}

		std::fstream write;
		write.open("src\\merge.txt", std::ios::out | std::ios::app);
		write << pool;
		write.close();
		pool = "";

	}

}