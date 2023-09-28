#pragma once

#include <atomic>
#include <iterator> 
#include <map>
#include <iostream>
#include <thread>
#include <mutex>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include <list>

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <string.h>

#include <set>
#include <io.h>
#include <windows.h>
#include <iomanip>
#include <psapi.h>
#include <cmath>

#include <memory>
#include <functional>
#include <utility>
#include <condition_variable>
#include <queue>
#include <future>

#pragma comment(lib,"psapi.lib")

#define M (4)
#define LIMIT_M_2 (M % 2 ? (M + 1)/2 : M/2)
#define readlength 100

#undef MOD_NAME
#define MOD_NAME ID_WASH

#pragma once

//项目基本设定
#define PROJECT_NAME NEXT_GENERATION_SEQUENCING
#define VERSION "0.1.0"

//调试设置
#ifdef _WIN32
	#define debugger system("pause")
#else
	#define debugger cin.get()
#endif // _WIN32

//B+树可选设置
/*B+树文件路径*/#define BPLUES_INIT_FILE_NAME "D:\\zhiju1.txt"



//ID清洗可选设置
#define ID_WASH_INIT_CHARACTERISTIC_FILE_NAME "G:\\Test\\characteristic-matrix.txt"
#define ID_WASH_INIT_MATCH_FILE_NAME "G:\\Test\\characteristic-matrix1.txt"
#define ID_WASH_OUTPUT_MATCH_FILE_NAME "G:\\Test\\file1.txt"
#define ID_WASH_OUTPUT_SEQUENCE_FILE_NAME "G:\\Test\\file2.txt"

namespace PROJECT_NAME
{
	namespace MOD_NAME

	{
		typedef struct BPlusNode* BPlusTree, * Position;
		typedef struct BPlusNode Node;
		typedef struct keyType KeyType;

		static std::mutex mymutex;
		static std::ifstream filein;
		static BPlusTree T;


		extern BPlusTree Initialize();

		extern BPlusTree Insert(BPlusTree T, KeyType Key);

		extern BPlusTree Remove(BPlusTree T, KeyType Key);

		extern BPlusTree Destroy(BPlusTree T);

		extern void Travel(BPlusTree T);

		extern void TravelData(BPlusTree T);

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

		int testFunc();
	}
}

