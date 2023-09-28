#include "../../include/IO/IO.h"


namespace _PROJECT_NAME_ 
{
	extern std::vector<_PROJECT_NAME_::KMEANS::Kmeans*>kmeans_vector;
	namespace _MOD_NAME_ 
	{
		int readFile(const std::string FileName)
		{
			int times = 20000;

			std::fstream fp;
			fp.open(FileName, std::ios::in);
			if (!fp.is_open()) { throw 1; }
		
			//设置缓冲区，默认参数
			const int BUF_SIZE = (const int)_PROJECT_NAME_::COMMAND_LINE::getIO_BUFF_SIZE();
			const int MATRIX_NUM = (const int)_PROJECT_NAME_::COMMAND_LINE::getMATRIX_NUM();
			char* temp_buf = new char[BUF_SIZE]();
			memset(temp_buf, '\0',BUF_SIZE);

			while ( fp.getline(temp_buf, (sizeof(char))*BUF_SIZE) )
			{
				//if (!(times--)) { break; }

				//ID
				std::string* ID = new std::string(temp_buf);

				//碱基
				fp.getline(temp_buf, (sizeof(char)) * BUF_SIZE);
				char* sequence_buf = new char[BUF_SIZE]();
				strcpy_s(sequence_buf, BUF_SIZE,temp_buf);
				
				

				//(Lambda)unravelMatrix拆解矩阵函数
				auto unravelMatrix = [=](char* buf,float* matrix) -> void
				{
					std::string temp;
					for (int i = 0,j = 0; buf[i] != '\0'; i++)
					{
						if (buf[i] != ',')
						{
							temp += buf[i];
						}
						else
						{
							matrix[j] = std::stof(temp);
							temp = "";
							if (matrix[j] == 0)break;
							j++;
						}
					}
				};

				//特征1
				fp.getline(temp_buf, (sizeof(char)) * BUF_SIZE);
				float* forward_matrix = new float[MATRIX_NUM]();
				unravelMatrix(temp_buf, forward_matrix);
				fp.getline(temp_buf, (sizeof(char)) * BUF_SIZE);
				
				//特征2
				fp.getline(temp_buf, (sizeof(char)) * BUF_SIZE);
				float* backward_matrix = new float[MATRIX_NUM]();
				unravelMatrix(temp_buf, backward_matrix);
				fp.getline(temp_buf, (sizeof(char)) * BUF_SIZE);

				std::string* temp_str = new std::string(sequence_buf);
				delete []sequence_buf;
				
				//空行
				fp.getline(temp_buf, (sizeof(char)) * BUF_SIZE);


				_PROJECT_NAME_::KMEANS::Kmeans* kmeans = new _PROJECT_NAME_::KMEANS::Kmeans(ID, temp_str, forward_matrix, backward_matrix);
				kmeans_vector.push_back(kmeans);
			}
		

			return 0;
		}
	
		int outputResult(std::multimap<std::string, std::string>*list,int code)
		{
			std::string BASE_FILE_NAME = _OUTPUT_FILE_NAME_ + std::to_string(code);
			BASE_FILE_NAME = (const std::string)BASE_FILE_NAME + ".txt";
			
			std::fstream fp;
			fp.open(BASE_FILE_NAME, std::ios::out | std::ios::trunc);
			if (!fp.is_open()) { throw 2; }

			for (auto iter = list->begin(); iter != list->end(); iter++)
			{
				fp << (*iter).first << "-" << (*iter).second<<"\n";
			}
		}

		//重载输出函数（备用）
		int outputResult(std::vector< std::vector<std::string*>*>* main_res_vector,
			std::vector< std::vector<std::string*>*>* main_res_vector_u,int code)
		{
			std::string BASE_FILE_NAME = _OUTPUT_FILE_NAME_ + std::to_string(code);
			BASE_FILE_NAME = (const std::string)BASE_FILE_NAME + ".txt";
			std::fstream fp;

			try {
				fp.open(BASE_FILE_NAME, std::ios::out | std::ios::trunc);
				if (!fp.is_open()) { std::cout << "Err\n"; throw 2; }
				else { std::cout << "here is open\n"; }
			}
			catch (int code) {
				_PROJECT_NAME_::EXCEPTION::outputErrorCode(code);
			}

			auto iter1 = main_res_vector->begin(); //kmeans
			static auto iter0 = kmeans_vector.begin();
			for (; iter1 != main_res_vector->end();)
			{
				fp << *(*iter0)->ID << "-";
				for (auto iter2 = (*iter1)->begin(); iter2 != (*iter1)->end(); iter2++)//矩阵特征值//30bp应该是第一个
				{
						fp << *(*iter2) << ",";
				}
				fp << "\n";
				iter0++;
				iter1++;
			}
			fp.close();

			return 0;

			std::string BASE_FILE_NAME_U = _OUTPUT_FILE_NAME_ + std::to_string(code);
			BASE_FILE_NAME_U = (const std::string)BASE_FILE_NAME_U + "-u.txt";
			std::fstream fp_u;

			try {
				fp_u.open(BASE_FILE_NAME_U, std::ios::out | std::ios::trunc);
				if (!fp_u.is_open()) { std::cout << "Err\n"; throw 2; }
				else { std::cout << "here is open\n"; }
			}
			catch (int code) {
				_PROJECT_NAME_::EXCEPTION::outputErrorCode(code);
			}

			

			auto oiter1 = main_res_vector_u->begin(); //kmeans
			auto oiter0 = kmeans_vector.begin();
			for (; oiter1 != main_res_vector_u->end();)
			{
				fp_u << *(*oiter0)->ID << "u-";
				for (auto iter2 = (*oiter1)->begin(); iter2 != (*oiter1)->end(); iter2++)//矩阵特征值//30bp应该是第一个
				{	
						fp_u << *(*iter2) << ",";	
					break;
				}
				fp_u << "\n";
				oiter0++;
				oiter1++;
			}
			fp_u.close();




			return 0;
		}
	}
}