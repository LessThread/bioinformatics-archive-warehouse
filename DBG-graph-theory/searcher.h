#pragma once
#include "public.h"
class searcher
{
public:
	unsigned id;
	Node* current;
	std::vector<Node*>road;
	std::vector<searcher*>son;//��ָ��ı������

	searcher(Node* current, unsigned id);
	searcher(Node* current,std::vector<Node*>&before,unsigned id);
	void addPath(Node* path);
};

