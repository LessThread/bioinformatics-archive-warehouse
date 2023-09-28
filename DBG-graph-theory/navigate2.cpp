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
	std::vector<searcher*> vNode;//改容器用于储存生成的各指针结构体
	searcher* g = new searcher(ptr,0);
	g->road.push_back(ptr);
	vNode.push_back(g);
	//初始化第一指针
	//创建一个弹出的缓冲区，创建一个添加的缓冲区；
	std::vector<searcher*> buf;
	
	//此循环遍历容器内的每个指针
	while (vNode.size()!=0)
	{
		//std::cout << "全部节点数量：" << vNode.size() << "\n";
		for (std::vector<searcher*>::iterator iter = vNode.begin(); iter != vNode.end();)
		{	

			//已达终点，抛出路径容器，生成路径
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
			
			//遇到了桥，生成字符串并标记
			//else if(((*iter)->current->next.size() == 1)&&((*iter)->current->next[0]->next.size() == 1))
			//{
			//	std::string* pstr = generateString((*iter)->road, T, bmaps);
			//	std::cout << "产生了wait于"<<(*iter)->current->inf<<"处\n";
			//	wait->push_back(std::pair<std::string, std::string>(*pstr, (*iter)->current->inf));
			//	//std::cout << "生成的字符串是：" << *pstr << "\n";
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

			//无分叉，直接前进
			else if ((*iter)->current->next.size() == 1)
			{
				
				(*iter)->road.push_back((*iter)->current->next[0]);
				(*iter)->current = (*iter)->current->next[0];
				iter++;

			}
			
			//有分叉，注意处理成环函数，不过这种情况应该可以有效避免复杂的指针回溯判断问题
			else
			{
				for (unsigned i = 1; i < (*iter)->current->next.size(); i++)
				{
					searcher* t = new searcher((*iter)->current, (*iter)->road,num++);
					t->road.push_back((*iter)->current->next[i]);
					t->current = (*iter)->current->next[i];
					
					//由于vector此时不能修改，以vNodes作为缓冲池暂存将在下一次计算的searcher对象,
					//不过应该也可以重新初始化迭代器
					buf.push_back(t);
				}

				(*iter)->road.push_back((*iter)->current->next[0]);
				(*iter)->current = (*iter)->current->next[0];
				iter++;
			}

			//std::cout << "\n";
		}


		//此片段将缓冲池中的元素加入原容器内，记得清空
		{
			for (std::vector<searcher*>::iterator is = buf.begin(); is != buf.end(); is++)
			{
				vNode.push_back(*is);
			}
			buf.clear();
		}

		//调用成环判断函数
		if (time++ % 1 == 0)
		{
			////system("pause");
			RingJudgment(vNode,Nvector);
		}

		if (time > 10000000) { std::cout << "错误处理：循环超 10000000 次，可能发生了死循环，错误行数是： navigate2.cpp 176"; system("pause"); }
		//////system("pause");
	}

}


void navigate2(std::vector<Node*>HeadNvector,
	std::vector<Node*>&bridge,
	std::vector<Node*>&Nvector,
	std::map < std::string, long long > bmaps,
	std::vector<std::string*> &fstr,//最终的str储存器
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

	


	//遍历普通头
	for (auto iHeader = HeadNvector.begin(); iHeader != HeadNvector.end(); iHeader++)
	{
		std::vector<std::string*>* str=new std::vector<std::string*>;
		std::vector<std::pair<std::string,std::string>>* wait = new std::vector<std::pair<std::string, std::string>>;
		
		//BFS(*iHeader, str, wait,BPlusT, bmaps,Nvector);

		HeadStr.push_back(str);
		HeadWait.push_back(wait);
		
		threads.push_back(std::thread(BFS, *iHeader, str, wait, BPlusT, bmaps, ref(Nvector)));

	}
	puts("遍历头节点完成\n"); //system("pause");

	//遍历桥断点
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


		//以下节点用于测试生成的字符串
		/*for (auto i = wait->begin(); i != wait->end(); i++)
		{
			std::cout << (*i).first << "---" << (*i).second << "\n";
		}*/
	}

	//等待着所有线程都返回后开始处理
	for (auto iter = threads.begin(); iter != threads.end(); ++iter)
	{
		iter->join();
	}

	puts("遍历桥节点完成\n"); //system("pause");


	std::vector<std::pair<std::string, std::pair<std::string, std::string>>>Fragment;//待进入拼合
	std::vector<std::pair<std::string, std::string>>inWait;//待拼合
	//重整桥断点片段并取出
	//取出普通头放入已完成容器中

	//取出待拼合放入容器中
	for (auto i = HeadWait.begin(); i != HeadWait.end(); i++)
	{
		//std::cout << (*i)->size()<<"$\n";
		for (auto j = (*i)->begin(); j != (*i)->end(); j++)
		{
			//std::cout << (*j).first <<"*" << (*j).second << "\n";
			inWait.push_back((*j));
		}
	}

	//重整两头断点的片段
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


	//这里是debug节点
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
	puts("重整完毕"); //system("pause");



	//重整完毕，准备拼接,思路：需要啥拼接啥上去，直到拼接完成为止，
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
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>>Fragment;//待进入拼合
	std::vector<std::pair<std::string, std::string>>inWait;//待拼合
	*/

}