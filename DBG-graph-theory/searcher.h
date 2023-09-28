#pragma once
#include "public.h"
class searcher
{
public:
	unsigned id;
	Node* current;
	std::vector<Node*>road;
	std::vector<searcher*>son;//子指针的标记容器

	searcher(Node* current, unsigned id);
	searcher(Node* current,std::vector<Node*>&before,unsigned id);
	void addPath(Node* path);
};

