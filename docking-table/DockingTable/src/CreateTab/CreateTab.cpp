#include "../../include/CreateTab/CreateTab.h"

extern std::mutex tlock;
namespace _PROJECT_NAME_
{
	extern std::vector<_PROJECT_NAME_::KMEANS::Kmeans*>kmeans_vector;
	extern std::multimap<float, std::string>back_map;

	namespace _MOD_NAME_
	{
		int createTab(int bp_num)
		{
			//��Ҫһ����ͻ���ģ�飨vector֧��������ʣ�

			int area_limit = -1;

			//�˶οɻ�Ϊ����
			switch (bp_num)
			{
			case 30:
				area_limit = 1; break;
			case 60:
				area_limit = 2; break;
			case 180:
				area_limit = 6; break;
			default:
				break;
			}

			const float ERROR_THRESHOLD = (_ERROR_THRESHOLD_ * 2) / 10;//�ⲽ��ϸ��
			std::vector<std::thread>threads;
			std::vector<std::thread>used_threads;

			//�������湤����map������
			std::map<float, std::vector<std::string*> >* cache_map = new std::map<float, std::vector<std::string*>>;

			//������
			std::vector< std::vector<std::string*>*>* main_res_vector = new std::vector< std::vector<std::string*>*>;
			std::vector< std::vector<std::string*>*>* main_res_vector_u = new std::vector< std::vector<std::string*>*>;

			unsigned count = 0;
			unsigned name = 0;
			for (auto iter = kmeans_vector.begin(); iter != kmeans_vector.end(); iter++)//��������kmeans
			{
				if (count % 200 == 0) 
				{
					std::cout <<"threads wait:" << count << "\n";
					for (auto wait_iter = threads.begin(); wait_iter != threads.end(); wait_iter++)
					{
						if(wait_iter->joinable())
						wait_iter->join();
					}

					threads.clear();

					if (count % 10000 == 0)
					{
						name++;
						std::cout << "file output:" << count << "\n";
						_PROJECT_NAME_::IO::outputResult(main_res_vector, main_res_vector_u,name);

						main_res_vector->clear();
						main_res_vector_u->clear();

						//�ͷ��ڴ棿
					}

				}

				std::vector<std::string*>* catch_vector = new std::vector<std::string*>;
				std::vector<std::string*>* catch_vector_u = new std::vector<std::string*>;

				main_res_vector->push_back(catch_vector);
				main_res_vector_u->push_back(catch_vector_u);


				threads.push_back(
					std::thread
					([=](std::vector<std::string*>*catch_vector, std::vector<std::string*>* catch_vector_u)
					{
						//ע�����ﴫ�ε������᲻��������
						int forward_matrix_iter = 0; 
						(*iter)->backward_matrix[forward_matrix_iter] != 0;

						float value = (*iter)->backward_matrix[forward_matrix_iter];//value��Ҫ������ֵ
						float upper_limit = value + ERROR_THRESHOLD;
						float lower_limit = value - ERROR_THRESHOLD;//�Ż�����(tag��)

						//�˴���ͨ��tag�Ż�,��һ������ӿ�
						//for (auto map_iter = back_map.begin(); map_iter != back_map.end(); map_iter++)
						//{
						//	//ע�⸡�����ĶԱ�,��ǿ��ת�����ͻ���warning
						//	if (value == (*map_iter).first)
						//	{
						//		catch_vector->push_back((map_iter)->second);
						//	}
						//	else if ((value - (*map_iter).first) < (0 - ERROR_THRESHOLD))
						//	{
						//		break;
						//	}
						//}

						//cache��ȡ

						if (cache_map->find(value) != cache_map->end())
						{
							*catch_vector = (cache_map->find(value))->second;
						}

						else
						{
							//STL����ͨ��find�ҵ�
							unsigned len = 0;
							unsigned max_len = back_map.count(value);
							for (auto map_iter = back_map.find(value); len < max_len; len++)
							{
								catch_vector->push_back(&(map_iter)->second);
								map_iter++;
							}

						}
						//�̼߳������
						
						//����1������ǰ1000��������2���������1000������Ҫ����������δ֪��
						if (count < 1000)
						{
							tlock.lock();
							cache_map->insert(std::pair<float,std::vector<std::string*>>(value, *catch_vector));
							tlock.unlock();
						}

					}, catch_vector,catch_vector_u));

				count++;

			}

			for (auto iter = threads.begin(); iter != threads.end(); iter++)
			{
				if(iter->joinable())
				(*iter).join();
			}
			
			std::cout << "file output\n";
			_PROJECT_NAME_::IO::outputResult(main_res_vector, main_res_vector_u,999);


			return 0;
		}
	}
}