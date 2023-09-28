#pragma once
#include"public.h"
#include "IO.h"
#include "navigate2.h"
#include <time.h>
#include "StorageInterface.h"
#include "dll.h"

extern "C" _declspec(dllexport)void start(std::string readName,std::string matchName)
{
    //readname为b+，match为对接表

    std::cout<<"We will get information from: " << readName << " and " << matchName;

    clock_t start, end;//定义clock_t变量
    start = clock();

    std::vector<std::thread>threads;

    //初始化b+树,并生成线程抛出
    std::map<std::string, long long>bmaps;
    BPlusTree* BPlusT = NULL;
    const std::string p = readName;
    threads.push_back(std::thread(call_init, p, &(BPlusT), std::ref(bmaps)));
    //call_init(p, &BPlusT, bmaps);

    ////system("pause");

    //创建指针容器，具体请求内存在io函数中
    std::vector<Node*>Nvector;
    std::map<std::string, unsigned>maps;//maps用于加快节点的查询和访问速度

    //读文件，并生成网络
    readFile(Nvector, maps,matchName);

    //分割子图，检查所有的开始节点和结束节点
    //注意：读vector容器时不能直接读【0】否则会引起下标越界

    std::vector<Node*>HeadNvector;
    std::vector<Node*>EndNvector;
    std::vector<Node*>bridge;

    SubgraphSegmentation(bridge, Nvector, HeadNvector, EndNvector);

    for (auto iter = threads.begin(); iter != threads.end(); ++iter)
    {
        iter->join();//等待着所有线程都返回后开始处理
    }


    //开始处理生成好的图
    {
        puts("节点：");
        for (auto i = Nvector.begin(); i != Nvector.end(); i++)
        {
            std::cout << (*i)->inf << " ";
        }

        puts("\n 头");
        for (auto i = HeadNvector.begin(); i != HeadNvector.end(); i++)
        {
            std::cout << (*i)->inf << " ";
        }
        puts("\n 桥");
        for (auto i = bridge.begin(); i != bridge.end(); i++)
        {
            std::cout << (*i)->inf << " ";
        }
        puts("\n 尾");
        for (auto i = EndNvector.begin(); i != EndNvector.end(); i++)
        {
            std::cout << (*i)->inf << " ";
        }

        puts("开始调用nav函数\n\n\n"); //system("pause");
    }

    std::vector<std::string*>str;
    navigate2(HeadNvector, bridge, Nvector, bmaps, str, BPlusT, EndNvector);

    for (auto ir = str.begin(); ir != str.end(); ir++)
    {
        std::cout << **ir << "\n####\n";
    }

    end = clock();   //结束时间
    std::cout << "time = " << double(end - start) / CLOCKS_PER_SEC << "s" << std::endl;


    creatFile(str);
    //system("pause");

}
