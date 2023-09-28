#pragma once
#include"public.h"
#include "IO.h"
#include "navigate2.h"
#include <time.h>
#include "StorageInterface.h"
#include "dll.h"

extern "C" _declspec(dllexport)void start(std::string readName,std::string matchName)
{
    //readnameΪb+��matchΪ�Խӱ�

    std::cout<<"We will get information from: " << readName << " and " << matchName;

    clock_t start, end;//����clock_t����
    start = clock();

    std::vector<std::thread>threads;

    //��ʼ��b+��,�������߳��׳�
    std::map<std::string, long long>bmaps;
    BPlusTree* BPlusT = NULL;
    const std::string p = readName;
    threads.push_back(std::thread(call_init, p, &(BPlusT), std::ref(bmaps)));
    //call_init(p, &BPlusT, bmaps);

    ////system("pause");

    //����ָ�����������������ڴ���io������
    std::vector<Node*>Nvector;
    std::map<std::string, unsigned>maps;//maps���ڼӿ�ڵ�Ĳ�ѯ�ͷ����ٶ�

    //���ļ�������������
    readFile(Nvector, maps,matchName);

    //�ָ���ͼ��������еĿ�ʼ�ڵ�ͽ����ڵ�
    //ע�⣺��vector����ʱ����ֱ�Ӷ���0������������±�Խ��

    std::vector<Node*>HeadNvector;
    std::vector<Node*>EndNvector;
    std::vector<Node*>bridge;

    SubgraphSegmentation(bridge, Nvector, HeadNvector, EndNvector);

    for (auto iter = threads.begin(); iter != threads.end(); ++iter)
    {
        iter->join();//�ȴ��������̶߳����غ�ʼ����
    }


    //��ʼ�������ɺõ�ͼ
    {
        puts("�ڵ㣺");
        for (auto i = Nvector.begin(); i != Nvector.end(); i++)
        {
            std::cout << (*i)->inf << " ";
        }

        puts("\n ͷ");
        for (auto i = HeadNvector.begin(); i != HeadNvector.end(); i++)
        {
            std::cout << (*i)->inf << " ";
        }
        puts("\n ��");
        for (auto i = bridge.begin(); i != bridge.end(); i++)
        {
            std::cout << (*i)->inf << " ";
        }
        puts("\n β");
        for (auto i = EndNvector.begin(); i != EndNvector.end(); i++)
        {
            std::cout << (*i)->inf << " ";
        }

        puts("��ʼ����nav����\n\n\n"); //system("pause");
    }

    std::vector<std::string*>str;
    navigate2(HeadNvector, bridge, Nvector, bmaps, str, BPlusT, EndNvector);

    for (auto ir = str.begin(); ir != str.end(); ir++)
    {
        std::cout << **ir << "\n####\n";
    }

    end = clock();   //����ʱ��
    std::cout << "time = " << double(end - start) / CLOCKS_PER_SEC << "s" << std::endl;


    creatFile(str);
    //system("pause");

}
