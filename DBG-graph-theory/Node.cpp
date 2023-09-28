#include "Node.h"
Node::Node(std::string inf)
{
	this->inf = inf;
};

void Node::addNext(Node* pNext)
{
	this->next.push_back(pNext);
};

void Node::addFront(Node* pFront)
{
	this->next.push_back(pFront);
};