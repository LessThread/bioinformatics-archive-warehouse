#include "navigate.h"

void goback(Node* ptr,std::vector<Node*>& road, std::vector<Node*>& fork,std::vector<std::string*>& str)
{
	if (ptr->front.size() == 0)
	{

	}
	else
	{

	}
}

void going(Node* ptr, std::vector<Node*>&road, std::vector<Node*>&fork, std::vector<std::string*>&str)
{
	if (ptr->next.size() == 1)
	{
		road.push_back(ptr->next[0]);
		return going(ptr->next[0], road,fork,str);
	}
	else if (ptr->next.size() == 0)
	{
		return goback(ptr,road, fork, str);
	}
	else//分叉情况
	{
		for (std::vector<Node*>::iterator iter = (ptr->next).begin();iter != (ptr->next).end();iter++)//寻找front容器内的所有未通过节点
		{
			if (vectorFind(fork, *iter)==0)//如果存在未走过的分叉节点，则进栈
			{
				fork.push_back(*iter);
				road.push_back(*iter);
				going(*iter, road, fork, str);
			}
		}

		return  goback(ptr, road, fork, str);//如果分叉的路走完了则退回
	}
}


void track(Node* HeadPtr, std::vector<std::string*>&str)
{
	Node* workptr = HeadPtr;
	std::map<Node*, unsigned>journey;//储存走过的路径
	std::vector<Node*>road;
	std::vector<Node*>fork;
	for (;;)
	{
		if (workptr->next.size()==0)//此路径结束，栈退回
		{
			
		}
		else if(workptr->next.size() == 1)//如果前方只有一个节点
		{
			workptr = workptr->next[0];
			road.push_back(workptr);
		}
		else//如果前方存在分叉
		{
			for (std::vector<Node*>::iterator iter = (workptr->next).begin();
				iter != (workptr->next).end();
				iter++)//寻找front容器内的所有未通过节点
			{
				//存在未通过路径节点，分叉进栈，单向通过
				if (journey.find(*iter) == journey.end())//在map没有找到此节点
				{
					journey.insert(std::pair<Node*, unsigned>(*iter, journey.size()));
					workptr = *iter;//添加至map并前进
					fork.push_back(workptr);

				}
				else//成环异常
				{

				}
			}
		}
	}
}

void work(Node* HeadPtr, std::vector<std::string*>& str)//计算每一个起始节点的封装函数，DFS是使用迭代还是递归？或者是定义指针类型，用广度搜索?
{
	std::vector<Node*>road;//记录路径
	std::vector<Node*>fork;//记录分叉
	std::vector<Node*>v;//分叉栈
	going(HeadPtr, road, fork,str);
}


//-----------------------------------------------------
//广度搜索写法

void BFS2(Node* ptr, std::vector<std::string*>& str)
{
	
}








void navigate(std::vector<Node*>HeadNvector, 
			  std::vector<Node*>Nvector,
			  std::map<std::string, unsigned>maps,
	          std::vector<std::string*>&str)//str要更改
{
	std::vector<Node*>::iterator iHeader;//所有起始节点容器的迭代器
	for (iHeader = HeadNvector.begin(); iHeader != HeadNvector.end(); iHeader++)
	{
		//计算每一个起始节点的封装函数
		//track(*iHeader, str,);
		//work(*iHeader, str);
		BFS2(*iHeader, str);
	}
}