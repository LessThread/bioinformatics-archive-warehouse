#include "PublicHeader.h"
#include "IO.h"
#include "thread.h"



int main()
{
    FileRead_and_BreakUp();//�˺���Ԥ������ܵ����ļ�

	unsigned uALL_LIN = 0;
	std::string KmeansSort_FileName = "src\\KmeansSort.txt";
	std::string BaseSort_FileName = "src\\BaseSort.txt";

	//�˶����ͬ���ļ�
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

	//��֯�߳�����
	std::fstream KmeansSort;
	KmeansSort.open(KmeansSort_FileName, std::ios::in);
	if (KmeansSort.fail())
	{
		std::cout << "open Kmeans Sort File err";
		exit(0);
	}
	std::vector<std::thread>threads;
	//


	//�����̲߳��׳�,�����buf�ǻ����飻
	while (KmeansSort.getline(buf[id % _BUF_NUM_], sizeof(char) * BUF_SIZE))
	{
		threads.push_back(std::thread(ThreadProcessor, buf[id % _BUF_NUM_], id));
		id++;
		uALL_LIN++;
	}
	//

	//ͬ�����ȴ��������̶߳�����
	for (auto iter = threads.begin(); iter != threads.end(); ++iter)
	{
		iter->join();
	}


	//��ʼ�����ı�
	mergeFile(uALL_LIN);
	//


	//�ͷ��ڴ�ռ�
	for (int i = 0; i < _BUF_NUM_; i++)
	{
		free(buf[i]);
	}
	free(buf);
	//

	std::cout << "program ends" << std::endl;
}
