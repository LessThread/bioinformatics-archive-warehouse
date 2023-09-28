#pragma once

#include<iostream>
#include<thread>
#include<mutex>
#include<algorithm>
#include <string>
#include<fstream>
#include<vector>

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include <set>
#include <io.h>
#include <windows.h>
#include <iomanip>
#include <psapi.h>
#include <cmath>

#undef MOD_NAME
#define MOD_NAME LIB 

//#pragma comment(lib,"psapi.lib")

#define M (4)
#define LIMIT_M_2 (M % 2 ? (M + 1)/2 : M/2)

//�����޸�Ϊ��
#define BPlusTree BPlusNode*
#define Position BPlusNode*
#define Node BPlusNode
#define keyType KeyType

namespace PROJECT_NAME
{
    namespace MOD_NAME
    {

        extern "C" struct __declspec(dllexport) keyType
        {
            int id;
            char xulie[1000];
        };

        extern "C" struct __declspec(dllexport) BPlusNode {
            int KeyNum;
            KeyType Key[M + 1];
            BPlusTree Children[M + 1];
            BPlusTree Next;
        };



        /* ��ʼ�� */
        __declspec(dllexport) BPlusTree Initialize();
        /* ���� */
        __declspec(dllexport) BPlusTree Insert(BPlusTree T, KeyType Key);
        /* ɾ�� */
        __declspec(dllexport) BPlusTree Remove(BPlusTree T, KeyType Key);
        /* ���� */
        __declspec(dllexport) BPlusTree Destroy(BPlusTree T);
        /* �����ڵ� */
        __declspec(dllexport) void Travel(BPlusTree T);
        /* ������Ҷ�ڵ������ */
        __declspec(dllexport) void TravelData(BPlusTree T);
        //���Ժ���
        __declspec(dllexport) int testFunc();

        __declspec(dllexport) void readFile(std::string file, BPlusTree& T);
    }
}



//int main()
//{
//    BPlusTree T;
//    KeyType X;
//    T = Initialize();
//    std::string path = "D:\\zhiju1.txt";
//    readFile(path , T);
//
//    X.id=5;
//    string a1=quickCheck(T, X);
//    cout<<a1<<endl;
//
//    return 0;
//}

