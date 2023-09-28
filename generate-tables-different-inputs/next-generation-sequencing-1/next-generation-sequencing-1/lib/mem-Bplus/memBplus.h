#pragma once
#include "../../main-settings.h"

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
#define MOD_NAME MEM_BPLUS

//#pragma comment(lib,"psapi.lib")

#define M (4)
#define LIMIT_M_2 (M % 2 ? (M + 1)/2 : M/2)


namespace PROJECT_NAME
{
    namespace MOD_NAME
    {
        typedef struct BPlusNode* BPlusTree, * Position;
        typedef struct BPlusNode Node;
        typedef struct keyType KeyType;

        struct keyType
        {
            int id;
            char xulie[1000];
        };

        struct BPlusNode {
            int KeyNum;
            KeyType Key[M + 1];
            BPlusTree Children[M + 1];
            BPlusTree Next;
        };



        /* 初始化 */
        BPlusTree Initialize();
        /* 插入 */
        BPlusTree Insert(BPlusTree T, KeyType Key);
        /* 删除 */
        BPlusTree Remove(BPlusTree T, KeyType Key);
        /* 销毁 */
        BPlusTree Destroy(BPlusTree T);
        /* 遍历节点 */
        void Travel(BPlusTree T);
        /* 遍历树叶节点的数据 */
        void TravelData(BPlusTree T);
        //测试函数
        int testFunc();
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

