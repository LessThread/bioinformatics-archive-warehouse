#include "PublicHeader.h"
#include "IO.h"
#include "thread.h"



int main()
{
    FileRead_and_BreakUp();//此函数预处理接受到的文件

	unsigned uALL_LIN = 0;
	std::string KmeansSort_FileName = "src\\KmeansSort.txt";
	std::string BaseSort_FileName = "src\\BaseSort.txt";

	//此段清空同名文件
	std::fstream temp;
	temp.open(BaseSort_FileName, std::ios::out | std::ios::trunc);
	if (temp)
	{
		temp << "";
		std::cout << "BaseSort_File opened\n";
	}
	else
	{
		std::cout << "open " << BaseSort_FileName << " err\n";
		exit(0);
	}
	temp.close();
	//

	unsigned id = 0;
	char** buf = (char**)malloc(sizeof(char*) * _BUF_NUM_);
	if (!buf)
	{
		std::cout << "request buffer failed";
		exit(0);
	}

	for (int i = 0; i < _BUF_NUM_; i++)
	{
		buf[i] = (char*)malloc(sizeof(char) * BUF_SIZE);
		memset(buf[i], '\0', BUF_SIZE);
	}

	//组织线程容器
	std::fstream KmeansSort;
	KmeansSort.open(KmeansSort_FileName, std::ios::in);
	if (KmeansSort.fail())
	{
		std::cout << "open Kmeans Sort File err";
		exit(0);
	}
	std::vector<std::thread>threads;
	//


	//生成线程并抛出,这里的buf是缓冲组；
	while (KmeansSort.getline(buf[id % _BUF_NUM_], sizeof(char) * BUF_SIZE))
	{
		threads.push_back(std::thread(ThreadProcessor, buf[id % _BUF_NUM_], id));
		id++;
		uALL_LIN++;
	}
	//

	//同步，等待着所有线程都返回
	for (auto iter = threads.begin(); iter != threads.end(); ++iter)
	{
		iter->join();
	}


	//开始处理文本
	mergeFile(uALL_LIN);
	//


	//释放内存空间
	for (int i = 0; i < _BUF_NUM_; i++)
	{
		free(buf[i]);
	}
	free(buf);
	//

	std::cout << "program ends" << std::endl;
}
