#include "searcher.h"
searcher::searcher(Node* current,std::vector<Node*>&before,unsigned id)
{
	this->current = current;
	this->id = id;
	//std::cout << "���ǹ��캯�����Ҽ̳е�������: ";
	for (auto i = before.begin(); i != before.end(); i++)
	{
		this->road.push_back(*i);
	}
}

searcher::searcher(Node* current, unsigned id)
{
	this->current = current;
	std::vector<Node*>road;
	this->id = id;
}

void searcher::addPath(Node* path)
{
	road.push_back(path);
}
