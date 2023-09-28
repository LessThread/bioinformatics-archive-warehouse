#include <contig/io.h>
#include <idx/parser.h>
#include <mutex>
#include <map>
#include <string>
namespace parse{
	namespace contig{
		namespace contig{
			//<(file, idx),(tid, cid)>
			static std::map<std::tuple<std::string, int>, std::tuple<int, int>> matches;
			static std::mutex guard;
			static std::tuple<std::tuple<int, int>, bool> lock_entry_lookup(const struct MatchRequest& request);
			static struct MatchResponse doReadsRequestFromFile(const MatchRequest& request);
			static bool is_test = false;
			static struct MatchResponse testReadsRequestFromFile(const MatchRequest& request);
			static struct MatchResponse realReadsRequestFromFile(const MatchRequest& request);

			using MatchIndex = ::parse::contig::idx::MatchIndex;
			static std::map<std::string, MatchIndex*> index_files;
			static std::mutex indexes_guard;

			void clear(){
				for(auto& [file, index]:index_files)
					delete index;
			}

			int get_file_limits(const std::string& file){
				if (is_test)
					return 8;
				std::lock_guard<std::mutex> guard(indexes_guard);
				auto iter = index_files.find(file);
				MatchIndex* index;
				if (iter == index_files.end()) {
					index = ::parse::contig::idx::create_matches_index(file);
					index_files[file] = index;
				}else
					index = iter -> second;
				return index->size();
			}

			struct MatchResponse MatchRequestFromFile(const struct MatchRequest& request){
				return doReadsRequestFromFile(request);
			}

			static std::tuple<std::tuple<int, int>, bool> lock_entry_lookup(const struct MatchRequest& request){
				std::lock_guard<std::mutex> g(guard);
				auto iter = matches.find(std::make_tuple(request.file, request.idx));
				if(iter == matches.end()){
					//is empty
					matches[std::make_tuple(request.file, request.idx)] = std::make_tuple(request.tid, request.cid);
					return { std::make_tuple(request.tid, request.cid), true};
				}else if(iter -> second == std::tie(request.tid, request.cid))
					return {iter -> second, false};
				else
					return {iter -> second, true};
			}

			static struct MatchResponse doReadsRequestFromFile(const MatchRequest& request){
				if(is_test)
					return testReadsRequestFromFile(request);
				else
					return realReadsRequestFromFile(request);
			}

			static struct MatchResponse realReadsRequestFromFile(const MatchRequest& request){
				struct MatchResponse response;
				auto [tag, valid] = lock_entry_lookup(request);
				if(!valid){
					response.valid = false;
					return response;
				}
				response.valid = true;
				auto [otid, ocid] = tag;
				response.tid = otid;
				response.cid = ocid;
				indexes_guard.lock();
				MatchIndex* index;
				auto iter = index_files.find(request.file);
				if(iter == index_files.end()){
					index = ::parse::contig::idx::create_matches_index(request.file);
					if(!index){
						response.valid = false;
						indexes_guard.unlock();
						return response;
					}
					index_files[request.file] = index;
				}else
					index = iter -> second;
				indexes_guard.unlock();
				auto meta_match = index -> get({request.file, request.idx});
				if(meta_match){
					response.valid = true;
					response.entry = {request.idx, *meta_match};
				}else
					response.valid = false;
				return response;
			}

			std::vector<struct MatchResponse> 
			MultiMatchRequestsFromFile(const std::vector<struct MatchRequest>& requests){
				std::vector<struct MatchResponse> responses;
				for(const auto& request:requests)
					responses.push_back(std::move(MatchRequestFromFile(request)));
				return responses;
			}
		}
		namespace contig{
			std::vector<std::vector<int>> fake_mappings = {
				{2,3,5},
				{0,5,7},
				{4,5,1},
				{2,6,7},
				{1,3,6},
				{0,3,7},
				{4,5,7},
				{2,4,5}
			};
			void init_test(){
				is_test = true;
			}
			void clear_test(){
				is_test = false;
			}
			static struct MatchResponse testReadsRequestFromFile(const MatchRequest& request){
				auto [id, valid] = lock_entry_lookup(request);
				auto [tid, cid] = id;
				struct MatchResponse response;
				if(!valid){
					response.valid = false;
					return response;
				}
				response.valid = true;
				response.tid = tid;
				response.cid = cid;
				response.entry = {request.idx, fake_mappings[request.idx]};
				return response;
			}
		}
	}
}