#include "../../include/Kmeans/Kmeans.h"

namespace _PROJECT_NAME_ {
	namespace _MOD_NAME_ {
		Kmeans::Kmeans(std::string* ID, std::string* sequence, float* forward_matrix, float* backward_matrix)
		{
			this->ID = ID;
			this->sequence = sequence;
			this->forward_matrix = forward_matrix;
			this->backward_matrix = backward_matrix;
			
		};
		Kmeans::~Kmeans()
		{
			delete(ID);
			delete(sequence);
			delete[]forward_matrix;
			delete[]backward_matrix;
		};
	}
}