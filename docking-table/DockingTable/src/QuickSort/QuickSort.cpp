#include "../../include/QuickSort/QuickSort.h"

namespace _PROJECT_NAME_
{
	extern std::vector<_PROJECT_NAME_::KMEANS::Kmeans*>kmeans_vector;
	extern std::multimap<float, std::string>back_map;//һ�� ����ֵ-ID �ļ�ֵ��
	namespace _MOD_NAME_
	{
		int generateSortUnit()
		{
			unsigned counter = 1;
			for (auto iter = kmeans_vector.begin(); iter != kmeans_vector.end(); iter++)
			{
				std::string* ID = (*iter)->ID;
				float* value = nullptr;
				if (counter % 1000 == 0) 
					{ std::cout<<"Sort:" << *ID << "\n"; }
				counter++;
				value = &(*iter)->forward_matrix[0];//�˴�������30bpʱʹ�ã�˼·ÿ�ο�����������
				back_map.insert(pair(*value, *ID));
			}

			return 0;
		}
	}
}