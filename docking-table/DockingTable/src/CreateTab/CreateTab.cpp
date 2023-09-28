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
			//需要一个冲突解决模块（vector支持随机访问）

			int area_limit = -1;

			//此段可换为计算
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

			const float ERROR_THRESHOLD = (_ERROR_THRESHOLD_ * 2) / 10;//这步有细节
			std::vector<std::thread>threads;
			std::vector<std::thread>used_threads;

			//创建缓存工作集map以提速
			std::map<float, std::vector<std::string*> >* cache_map = new std::map<float, std::vector<std::string*>>;

			//总容器
			std::vector< std::vector<std::string*>*>* main_res_vector = new std::vector< std::vector<std::string*>*>;
			std::vector< std::vector<std::string*>*>* main_res_vector_u = new std::vector< std::vector<std::string*>*>;

			unsigned count = 0;
			unsigned name = 0;
			for (auto iter = kmeans_vector.begin(); iter != kmeans_vector.end(); iter++)//遍历所有kmeans
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

						//释放内存？
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
						//注意这里传参迭代器会不会有问题
						int forward_matrix_iter = 0; 
						(*iter)->backward_matrix[forward_matrix_iter] != 0;

						float value = (*iter)->backward_matrix[forward_matrix_iter];//value是要搜索的值
						float upper_limit = value + ERROR_THRESHOLD;
						float lower_limit = value - ERROR_THRESHOLD;//优化备用(tag法)

						//此处可通过tag优化,留一个计算接口
						//for (auto map_iter = back_map.begin(); map_iter != back_map.end(); map_iter++)
						//{
						//	//注意浮点数的对比,不强制转换类型会有warning
						//	if (value == (*map_iter).first)
						//	{
						//		catch_vector->push_back((map_iter)->second);
						//	}
						//	else if ((value - (*map_iter).first) < (0 - ERROR_THRESHOLD))
						//	{
						//		break;
						//	}
						//}

						//cache快取

						if (cache_map->find(value) != cache_map->end())
						{
							*catch_vector = (cache_map->find(value))->second;
						}

						else
						{
							//STL容器通过find找到
							unsigned len = 0;
							unsigned max_len = back_map.count(value);
							for (auto map_iter = back_map.find(value); len < max_len; len++)
							{
								catch_vector->push_back(&(map_iter)->second);
								map_iter++;
							}

						}
						//线程计算结束
						
						//方案1，缓存前1000条；方案2，缓存最近1000条（需要加锁，性能未知）
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