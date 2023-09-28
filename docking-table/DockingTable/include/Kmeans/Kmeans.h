#pragma once

#include "../Settings.h"
#include <string>
#include <iostream>
#include <vector>

#undef _MOD_NAME_
#define _MOD_NAME_ KMEANS



namespace _PROJECT_NAME_ {
	namespace _MOD_NAME_ {
		class Kmeans
		{
		public:
			std::string* ID;
			std::string* sequence;
			float* forward_matrix;
			float* backward_matrix;

			Kmeans(std::string* ID, std::string* sequence, float* forward_matrix, float* backward_matrix);
			~Kmeans();

		private:

		};

		
	}
}