#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <iostream>
#include <fstream>  
#include <string>
#include <set>
#include <io.h>
#include <windows.h>
#include <iomanip>
#include <psapi.h>
#include <cmath>
#include <map>
#pragma comment(lib,"psapi.lib")

#define M (4)
#define LIMIT_M_2 (M % 2 ? (M + 1)/2 : M/2)

typedef struct BPlusNode* BPlusTree, * Position;
typedef struct BPlusNode bNode;
typedef struct keyType KeyType;

BPlusTree* T_init(std::string p, std::map<std::string, long long>&maps);
std::string findSeq(unsigned id, BPlusTree* T);