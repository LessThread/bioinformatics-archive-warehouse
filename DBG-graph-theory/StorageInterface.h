#pragma once
#include "public.h";
#include "bplus.h"
//#pragma comment(lib,"StaticLib1.lib")
std::string find_seq(std::string i, BPlusTree* T, std::map < std::string, long long > bmaps);
void call_init(std::string str, BPlusTree** BPlusT, std::map<std::string, long long>&bmaps);