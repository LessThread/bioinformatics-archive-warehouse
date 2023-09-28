//mapping tables I/O
#pragma once
#include "types.h"
#include <string>
namespace parse{
	namespace contig{
		namespace contig{
			struct MatchRequest{
				int tid;//thread id
				int cid;//computation id
				int idx;//reads id
				std::string file;//file id
			};
			struct MatchResponse {
				bool valid;
				int tid;
				int cid;
				MatchEntry entry;
			};

			struct MatchResponse MatchRequestFromFile(const struct MatchRequest& request);
			
			std::vector<struct MatchResponse> 
				MultiMatchRequestsFromFile(const std::vector<struct MatchRequest>& requests);

			void clear();
			int get_file_limits(const std::string& file);
		}
		namespace contig{
			void init_test();
			void clear_test();
		}
	}
}