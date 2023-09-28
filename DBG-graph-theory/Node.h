#pragma once
#include"public.h"
class Node
{
public:
	std::vector<Node*>next;
	std::vector<Node*>front;
	std::string  inf;

	Node(std::string inf);
	void addNext(Node* pNext);
	void addFront(Node* pFront);
};

