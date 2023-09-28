#include "operate.h"
Node* FindORCreateNode(std::string s_id, std::vector<Node*>& Nvector, std::map<std::string, unsigned>& maps)
{
	std::map<std::string, unsigned>::iterator find_i;
	find_i = maps.find(s_id);

	if (find_i == maps.end())//若不存在则创建,注意num是指针类型
	{
		std::cout << s_id << "  not exist,creat ";

		//这里请求的空间可能在下个函数被释放，所以需要使用静态变量，或者mallloc new等
		//大坑：vector是将指针指向的内拷贝至容器内，而不是移动，使得容器内的元素和产生的临时元素分别指向两个内存
		//在之后的赋值修改中产生严重错误；

		Node* body = new Node(s_id);
		Nvector.push_back(body);
		maps.insert(std::pair<std::string, unsigned>(s_id, Nvector.size()-1));

		std::cout << body->inf << std::endl;

		return body;
	}
	else
	{
		std::cout << s_id << " exist map in: ";
		std::cout << find_i->first << "*" << find_i->second << std::endl;
		return Nvector[find_i->second];
	}
}

Node* link(std::string pointer, Node* bodys, std::map<std::string, unsigned>& maps, std::vector<Node*>& Nvector)
{
	std::string poi = pointer;
	Node* g = FindORCreateNode(poi,Nvector ,maps);

	std::cout << "bodys: " << bodys->inf << "-------";
	std::cout << "link: " << pointer << std::endl;

	bodys->next.push_back(g);
	g->front.push_back(bodys);

	return bodys;
}

bool vectorFind(std::vector<Node*> v,Node* f)
{
	for (std::vector<Node*>::iterator i = v.begin(); i != v.end(); i++)
	{
		if (*i == f)
		{
			return 1;
		}
	}

	return 0;
}


//子图分割函数
void SubgraphSegmentation(std::vector<Node*>&bridge,std::vector<Node*>Nvector,std::vector<Node*>&HeadNvector,std::vector<Node*>&EndNvector)
{

	//通过前指针为空白判断节点是起始节点，后指针为空判断为终止节点,前后都为一的时判断为桥
	for (std::vector<Node*>::iterator Nvi = Nvector.begin(); Nvi != Nvector.end(); Nvi++)
	{
		if ((*(*Nvi)).front.size() == 0)
		{
			HeadNvector.push_back(*Nvi);
		}

		else if ((*(*Nvi)).next.size() == 0)
		{
			EndNvector.push_back(*Nvi);
		}

		else if (((*(*Nvi)).front.size() == 1) && ((*(*Nvi)).next.size() == 1))
		{
			(*Nvi)->front[0]->next.pop_back();
			bridge.push_back(*Nvi);
			//桥也需要加入遍历队列,但是标记
		}
	}
	




}