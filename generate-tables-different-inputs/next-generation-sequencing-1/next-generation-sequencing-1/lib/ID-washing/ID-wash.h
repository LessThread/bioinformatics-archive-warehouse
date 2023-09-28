#pragma once
#include "../../main-settings.h"

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

