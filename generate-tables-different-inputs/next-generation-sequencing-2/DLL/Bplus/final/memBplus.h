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

//别名修改为宏
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



        /* 初始化 */
        __declspec(dllexport) BPlusTree Initialize();
        /* 插入 */
        __declspec(dllexport) BPlusTree Insert(BPlusTree T, KeyType Key);
        /* 删除 */
        __declspec(dllexport) BPlusTree Remove(BPlusTree T, KeyType Key);
        /* 销毁 */
        __declspec(dllexport) BPlusTree Destroy(BPlusTree T);
        /* 遍历节点 */
        __declspec(dllexport) void Travel(BPlusTree T);
        /* 遍历树叶节点的数据 */
        __declspec(dllexport) void TravelData(BPlusTree T);
        //测试函数
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

