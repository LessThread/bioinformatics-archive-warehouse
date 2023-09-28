/**
 * @brief      Reads I/O and parsing
 * @author     Ji Xiangyu
 * @date       2022.8.18
 */
#pragma once
#include <string>
#include "types.h"
#include <vector>
namespace parse{
	namespace contig{
		namespace reads{

			//request for a cooked reads
			//note that here id refer to speicifier
			struct ReadsRequest{
				int tid;//thread id
				int cid;//computation id
				int idx;//line id
				std::string file;//file id
			};

			struct ReadsResponse{
				bool valid;//data may be invalid such as out-of-bound
				int idx;
				int otid;//owner tid
				int ocid;//owner cid
				struct Reads reads;
			};
			//try to read a cooked read from file
			struct ReadsResponse ReadsRequestFromFile(const struct ReadsRequest& request);
			//batch read support
			std::vector<struct ReadsResponse> 
				MultiReadsRequestsFromFile(const std::vector<struct ReadsRequest>& requests);
			void clear();
		}

		namespace reads{
			//test only
			void init_test();
			void clear_test();
		}
	}
}