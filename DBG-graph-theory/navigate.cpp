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
	else//�ֲ����
	{
		for (std::vector<Node*>::iterator iter = (ptr->next).begin();iter != (ptr->next).end();iter++)//Ѱ��front�����ڵ�����δͨ���ڵ�
		{
			if (vectorFind(fork, *iter)==0)//�������δ�߹��ķֲ�ڵ㣬���ջ
			{
				fork.push_back(*iter);
				road.push_back(*iter);
				going(*iter, road, fork, str);
			}
		}

		return  goback(ptr, road, fork, str);//����ֲ��·���������˻�
	}
}


void track(Node* HeadPtr, std::vector<std::string*>&str)
{
	Node* workptr = HeadPtr;
	std::map<Node*, unsigned>journey;//�����߹���·��
	std::vector<Node*>road;
	std::vector<Node*>fork;
	for (;;)
	{
		if (workptr->next.size()==0)//��·��������ջ�˻�
		{
			
		}
		else if(workptr->next.size() == 1)//���ǰ��ֻ��һ���ڵ�
		{
			workptr = workptr->next[0];
			road.push_back(workptr);
		}
		else//���ǰ�����ڷֲ�
		{
			for (std::vector<Node*>::iterator iter = (workptr->next).begin();
				iter != (workptr->next).end();
				iter++)//Ѱ��front�����ڵ�����δͨ���ڵ�
			{
				//����δͨ��·���ڵ㣬�ֲ��ջ������ͨ��
				if (journey.find(*iter) == journey.end())//��mapû���ҵ��˽ڵ�
				{
					journey.insert(std::pair<Node*, unsigned>(*iter, journey.size()));
					workptr = *iter;//�����map��ǰ��
					fork.push_back(workptr);

				}
				else//�ɻ��쳣
				{

				}
			}
		}
	}
}

void work(Node* HeadPtr, std::vector<std::string*>& str)//����ÿһ����ʼ�ڵ�ķ�װ������DFS��ʹ�õ������ǵݹ飿�����Ƕ���ָ�����ͣ��ù������?
{
	std::vector<Node*>road;//��¼·��
	std::vector<Node*>fork;//��¼�ֲ�
	std::vector<Node*>v;//�ֲ�ջ
	going(HeadPtr, road, fork,str);
}


//-----------------------------------------------------
//�������д��

void BFS2(Node* ptr, std::vector<std::string*>& str)
{
	
}








void navigate(std::vector<Node*>HeadNvector, 
			  std::vector<Node*>Nvector,
			  std::map<std::string, unsigned>maps,
	          std::vector<std::string*>&str)//strҪ����
{
	std::vector<Node*>::iterator iHeader;//������ʼ�ڵ������ĵ�����
	for (iHeader = HeadNvector.begin(); iHeader != HeadNvector.end(); iHeader++)
	{
		//����ÿһ����ʼ�ڵ�ķ�װ����
		//track(*iHeader, str,);
		//work(*iHeader, str);
		BFS2(*iHeader, str);
	}
}