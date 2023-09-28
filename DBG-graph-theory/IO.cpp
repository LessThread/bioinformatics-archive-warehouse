#include "IO.h"
void readFile(std::vector<Node*>& Nvector ,std::map<std::string, unsigned>& maps,std::string name)
{
	const unsigned TempSize = 2000;

	std::fstream fp;
	fp.open(name, std::ios::in);

	//temp是每行所设的缓存区，大小为TempSize,可以是初始化形式或者请求内存形式
	char* temp = (char*)malloc(sizeof(char) * TempSize);
	memset(temp, '\0', TempSize);

	while (fp.getline(temp, sizeof(char) * TempSize))
	{
		unsigned i = 0;
		std::string s_id;

		//获取line和id并合并为string形式，成为类中的inf
		for (int j = 0; i < TempSize; i++)
		{
			if (temp[i] == '-')
			{
				i++;
				break;
			}
			else
			{
				s_id += temp[i];
			}
		}
	
		std::cout << "id:" << s_id << "\n";
		//////system("pause");

		//查询有无对应节点，若有，则增加尾节点；若无，则创建
		Node* body = FindORCreateNode(s_id, Nvector,maps);

		std::string poi="";
		//获取各尾指针并链接
		for (int j = 0; i < TempSize && temp[i] != '\0'; i++)
		{
		

			if (temp[i] == ' ')
			{
				link(poi, body, maps, Nvector);
				poi = "";
			}
			else
			{
				poi += temp[i];
			}

			if (temp[i + 1] == '\0')
			{
				link(poi, body, maps, Nvector);
				break;
			}
		}
	}

}

void creatFile(std::vector<std::string*>str)
{
	std::set<std::string> myset;
	for (auto i = str.begin(); i != str.end(); i++)
	{
		myset.insert(**i);
	}


	std::fstream fp;
	unsigned num=0;
	fp.open(".\\src\\res.txt", std::ios::out | std::ios::trunc);
	for (auto i = myset.begin(); i != myset.end(); i++)
	{
		//std::string x = **i;
		fp << num++ << ":";
		fp << *i << "\n";
	}
	fp.close();
}