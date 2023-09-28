#include "navigate2.h"

std::string* generateString(std::vector<Node*>road, BPlusTree* T,std::map < std::string, long long > bmaps )
{
	std::string* psrt = new std::string;
	for (std::vector<Node*>::iterator iroad = road.begin(); iroad != road.end(); iroad++)
	{
		*psrt += find_seq((*iroad)->inf, T, bmaps);
		std::cout << *(psrt)<<"\n";
		//*psrt += (*iroad)->inf;
		//*psrt += "->";
	}
	return psrt;
}

void delRing(std::vector<searcher*>& sea,std::string j, std::vector<Node*>& Nvector)
{
	for (auto i = Nvector.begin(); i != Nvector.end(); )
	{
		if ((*i)->inf == j)
		{
			std::cout<<"we delete "<<(*i)->inf<<"\n";
			(*i)->next.clear();
			puts("return\n");
			return;
		}

		else i++;
	}
}

void RingJudgment(std::vector<searcher*>&sea,std::vector<Node*>&Nvector)
{

	for (auto i = sea.begin(); i != sea.end(); i++)
	{
		bool key = false;
		std::multiset<std::string> mymultiset;
		std::cout << "check id: " << (*i)->id << "\n";
		for (auto j = (*i)->road.begin(); j != (*i)->road.end(); j++)
		{
			mymultiset.insert((*j)->inf);

			if (mymultiset.count((*j)->inf) > 1)
			{
				delRing(sea, (* j)->inf ,Nvector);
				std::cout << "del " << (*i)->id << "\n";
				continue;
				key = true;
				//break;
			}
		}

		if (key)
		{
			i = sea.erase(i);
		}

		//////system("pause");
	}

}

void BFS(
	Node* ptr, 
	std::vector<std::string*> *str, 
	std::vector<std::pair<std::string, std::string>> *wait,
	BPlusTree* T, 
	std::map < std::string, long long > bmaps, 
	std::vector<Node*>& Nvector)
{
	unsigned num = 1;
	unsigned time = 1;
	std::vector<searcher*> vNode;//���������ڴ������ɵĸ�ָ��ṹ��
	searcher* g = new searcher(ptr,0);
	g->road.push_back(ptr);
	vNode.push_back(g);
	//��ʼ����һָ��
	//����һ�������Ļ�����������һ����ӵĻ�������
	std::vector<searcher*> buf;
	
	//��ѭ�����������ڵ�ÿ��ָ��
	while (vNode.size()!=0)
	{
		//std::cout << "ȫ���ڵ�������" << vNode.size() << "\n";
		for (std::vector<searcher*>::iterator iter = vNode.begin(); iter != vNode.end();)
		{	

			//�Ѵ��յ㣬�׳�·������������·��
			if ((*iter)->current->next.size() == 0)
			{
				std::string* pstr = generateString((*iter)->road,T,bmaps);
				auto pa = std::pair<std::string, std::string>(*pstr, (*iter)->current->inf);
				wait->push_back(pa);


				if ((iter)==vNode.begin())
				{
					iter = vNode.erase(iter);
				}
				else
				{
					iter = vNode.erase(iter);
					iter--;
				}
				
			}
			
			//�������ţ������ַ��������
			//else if(((*iter)->current->next.size() == 1)&&((*iter)->current->next[0]->next.size() == 1))
			//{
			//	std::string* pstr = generateString((*iter)->road, T, bmaps);
			//	std::cout << "������wait��"<<(*iter)->current->inf<<"��\n";
			//	wait->push_back(std::pair<std::string, std::string>(*pstr, (*iter)->current->inf));
			//	//std::cout << "���ɵ��ַ����ǣ�" << *pstr << "\n";
			//	if ((iter) == vNode.begin())
			//	{
			//		iter = vNode.erase(iter);
			//	}
			//	else
			//	{
			//		iter = vNode.erase(iter);
			//		iter--;
			//	}
			//}

			//�޷ֲ棬ֱ��ǰ��
			else if ((*iter)->current->next.size() == 1)
			{
				
				(*iter)->road.push_back((*iter)->current->next[0]);
				(*iter)->current = (*iter)->current->next[0];
				iter++;

			}
			
			//�зֲ棬ע�⴦��ɻ������������������Ӧ�ÿ�����Ч���⸴�ӵ�ָ������ж�����
			else
			{
				for (unsigned i = 1; i < (*iter)->current->next.size(); i++)
				{
					searcher* t = new searcher((*iter)->current, (*iter)->road,num++);
					t->road.push_back((*iter)->current->next[i]);
					t->current = (*iter)->current->next[i];
					
					//����vector��ʱ�����޸ģ���vNodes��Ϊ������ݴ潫����һ�μ����searcher����,
					//����Ӧ��Ҳ�������³�ʼ��������
					buf.push_back(t);
				}

				(*iter)->road.push_back((*iter)->current->next[0]);
				(*iter)->current = (*iter)->current->next[0];
				iter++;
			}

			//std::cout << "\n";
		}


		//��Ƭ�ν�������е�Ԫ�ؼ���ԭ�����ڣ��ǵ����
		{
			for (std::vector<searcher*>::iterator is = buf.begin(); is != buf.end(); is++)
			{
				vNode.push_back(*is);
			}
			buf.clear();
		}

		//���óɻ��жϺ���
		if (time++ % 1 == 0)
		{
			////system("pause");
			RingJudgment(vNode,Nvector);
		}

		if (time > 10000000) { std::cout << "������ѭ���� 10000000 �Σ����ܷ�������ѭ�������������ǣ� navigate2.cpp 176"; system("pause"); }
		//////system("pause");
	}

}


void navigate2(std::vector<Node*>HeadNvector,
	std::vector<Node*>&bridge,
	std::vector<Node*>&Nvector,
	std::map < std::string, long long > bmaps,
	std::vector<std::string*> &fstr,//���յ�str������
	BPlusTree* BPlusT,
	std::vector<Node*>EndNvector
)
{
	std::vector<std::thread>threads;
	unsigned num = 0;
	
	std::vector<std::vector<std::string*>*> HeadStr;
	std::vector<  std::vector<std::pair<std::string, std::string>>*  >HeadWait;
	
	std::vector<std::pair<std::string,std::vector<std::string*>*>>BridgeStr;
	std::vector<std::pair< std::string, std::vector<std::pair<std::string, std::string>>* >  > BridgeWait;

	


	//������ͨͷ
	for (auto iHeader = HeadNvector.begin(); iHeader != HeadNvector.end(); iHeader++)
	{
		std::vector<std::string*>* str=new std::vector<std::string*>;
		std::vector<std::pair<std::string,std::string>>* wait = new std::vector<std::pair<std::string, std::string>>;
		
		//BFS(*iHeader, str, wait,BPlusT, bmaps,Nvector);

		HeadStr.push_back(str);
		HeadWait.push_back(wait);
		
		threads.push_back(std::thread(BFS, *iHeader, str, wait, BPlusT, bmaps, ref(Nvector)));

	}
	puts("����ͷ�ڵ����\n"); //system("pause");

	//�����Ŷϵ�
	for (auto iBridge = bridge.begin(); iBridge != bridge.end(); iBridge++)
	{
		num++;
		std::vector<std::string*>* str = new std::vector<std::string*>;
		std::vector<std::pair<std::string, std::string>>* wait = new std::vector<std::pair<std::string, std::string>>;

		auto f = std::pair<std::string, std::vector<std::pair<std::string, std::string>>* >(((*iBridge)->front[0]->inf), wait);
		BridgeWait.push_back(f);
		BridgeStr.push_back(std::pair< std::string, std::vector<std::string*>*>((*iBridge)->inf,str));

		threads.push_back(std::thread(BFS, *iBridge, str, wait, BPlusT, bmaps, ref(Nvector)));
		//BFS(*iBridge, str, wait, BPlusT, bmaps, Nvector);


		//���½ڵ����ڲ������ɵ��ַ���
		/*for (auto i = wait->begin(); i != wait->end(); i++)
		{
			std::cout << (*i).first << "---" << (*i).second << "\n";
		}*/
	}

	//�ȴ��������̶߳����غ�ʼ����
	for (auto iter = threads.begin(); iter != threads.end(); ++iter)
	{
		iter->join();
	}

	puts("�����Žڵ����\n"); //system("pause");


	std::vector<std::pair<std::string, std::pair<std::string, std::string>>>Fragment;//������ƴ��
	std::vector<std::pair<std::string, std::string>>inWait;//��ƴ��
	//�����Ŷϵ�Ƭ�β�ȡ��
	//ȡ����ͨͷ���������������

	//ȡ����ƴ�Ϸ���������
	for (auto i = HeadWait.begin(); i != HeadWait.end(); i++)
	{
		//std::cout << (*i)->size()<<"$\n";
		for (auto j = (*i)->begin(); j != (*i)->end(); j++)
		{
			//std::cout << (*j).first <<"*" << (*j).second << "\n";
			inWait.push_back((*j));
		}
	}

	//������ͷ�ϵ��Ƭ��
	for (auto i = BridgeWait.begin(); i != BridgeWait.end(); i++)
	{
		for (auto j = (*i).second->begin(); j != (*i).second->end(); j++)
		{
			Fragment.push_back(std::pair<std::string, std::pair<std::string, std::string>>((*i).first, (*j)));
		}
	}

	for (auto i = BridgeStr.begin(); i != BridgeStr.end(); i++)
	{
		puts("called");std::cout<<(*i).second->size()<<"\n";
	
		for (auto j = (*i).second->begin(); j != (*i).second->end(); j++)
		{
			Fragment.push_back(std::pair<std::string, std::pair<std::string, std::string>>
				((*i).first, std::pair<std::string, std::string>(*(*j), "end")));
		}
	}

	for (auto i = Fragment.begin(); i != Fragment.end(); i++)
	{
		for (auto j = EndNvector.begin(); j != EndNvector.end(); j++)
		{
			if ((*j)->inf == (*i).second.second)
			{
				(*i).second.second = "end";
			}
		}
	}


	//������debug�ڵ�
	puts("inwait:\n");
	for (auto i = inWait.begin(); i != inWait.end(); i++)
	{
		std::cout << (*i).first << "###" << (*i).second << "\n";
	}
	//system("pause");
	puts("fragment:\n");
	for (auto i = Fragment.begin(); i != Fragment.end(); i++)
	{
		std::cout << (*i).first << "###" << (*i).second.first << "###" << (*i).second.second << "\n";
	}
	puts("�������"); //system("pause");



	//������ϣ�׼��ƴ��,˼·����Ҫɶƴ��ɶ��ȥ��ֱ��ƴ�����Ϊֹ��
	if (Fragment.size() == 0)
	{
		for (auto i = inWait.begin(); i != inWait.end(); i++)
		{
			std::string* pstr = new std::string((*i).first);
			fstr.push_back(pstr);
		}
	}

	while (inWait.size() != 0)
	{
		std::vector<std::pair<std::string, std::string>>inWaitBuf;
		for (auto i = inWait.begin(); i != inWait.end(); )
		{
			for (auto j = Fragment.begin(); j != Fragment.end(); j++)
			{
				if ((*j).first == (*i).second)
				{
					std::string* p1 = new std::string((*j).second.second);
					std::string* p2 = new std::string((*i).first);
					*p2 += (*j).second.first;
					auto f = std::pair<std::string, std::string>(*p2, *p1);
					inWaitBuf.push_back(f);
				}
			}

			if ((*i).second == "end")
			{
				std::string* p = new std::string((*i).first);
				fstr.push_back(p);
				std::cout << "\nfstr size: " << fstr.size();
				std::cout << "did  " << *p << "\n";
			}

			i = inWait.erase(i);

		}

		for (auto i = inWaitBuf.begin(); i != inWaitBuf.end(); i++)
		{
			inWait.push_back((*i));
		}
	}
	

	std::cout<<"\nfstr size: " << fstr.size();
	//system("pause");
	
	/*
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>>Fragment;//������ƴ��
	std::vector<std::pair<std::string, std::string>>inWait;//��ƴ��
	*/

}