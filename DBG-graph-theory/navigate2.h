#pragma once
#include"public.h"
#include "operate.h"
#include "searcher.h"
#include "StorageInterface.h"

void navigate2(std::vector<Node*>HeadNvector,
	std::vector<Node*>&bridge,
	std::vector<Node*>& Nvector,
	std::map < std::string, long long > bmaps,
	std::vector<std::string*>& fstr,//×îÖÕµÄstr´¢´æÆ÷
	BPlusTree* BPlusT,
	std::vector<Node*>EndNvector
);
