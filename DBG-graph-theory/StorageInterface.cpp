#include "StorageInterface.h"
std::string find_seq(std::string i, BPlusTree* T, std::map < std::string, long long > bmaps)
{
	/*unsigned id = stol(i);
	std::cout << "ËÑË÷£º" << id << "\n";*/
	if (bmaps.find(i) == bmaps.end())
	{
		std::cout << "Err in " << i << std::endl;
		////system("pause");
	}
	unsigned id = (unsigned)bmaps.find(i)->second;
	return findSeq(id, T);
}
void call_init(std::string str, BPlusTree** BPlusT, std::map<std::string, long long>&bmaps)
{
	*BPlusT=T_init(".\\src\\reads.txt", bmaps);
}