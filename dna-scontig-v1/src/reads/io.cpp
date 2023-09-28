#include <reads/io.h>
#include <idx/parser.h>
#include <map>
#include <mutex>

namespace parse{
	namespace contig{
		namespace reads{
			//<(file, idx),(tid, cid)>
			static std::map<std::tuple<std::string,int>, std::tuple<int, int>> slices;
			using ReadsIndex = ::parse::contig::idx::ReadsIndex;
			static std::map<std::string, ReadsIndex*> files;
			static std::mutex files_guard;
			static std::map<std::string, FILE*> reads_files;
			static std::mutex reads_files_guard;
			static std::mutex guard;
			static std::tuple<std::tuple<int, int>,bool> lock_entry_lookup(const ReadsRequest& request);
			static struct ReadsResponse doReadsRequestFromFile(const ReadsRequest& request);
			static bool is_test = false;
			static struct ReadsResponse testReadsRequestFromFile(const ReadsRequest& request);
			static struct ReadsResponse realReadsRequestFromFile(const ReadsRequest& request);
			struct ReadsResponse ReadsRequestFromFile(const ReadsRequest& request){
				return doReadsRequestFromFile(request);
			}

			void clear(){
				for(auto& [file, index]:files)
					delete index;
				for(auto& [file, stream]:reads_files)
					fclose(stream);
			}

			static std::tuple<std::tuple<int, int>, bool> lock_entry_lookup(const ReadsRequest& request){
				std::lock_guard<std::mutex> g(guard);
				auto iter = slices.find(std::make_tuple(request.file, request.idx));
				if(iter == slices.end()){
					//is empty
					slices[std::make_tuple(request.file,request.idx)] = std::make_tuple(request.tid, request.cid);
					return {std::make_tuple(request.tid, request.cid), true};
				}else if(iter -> second == std::tie(request.tid, request.cid))
					return {iter -> second, false};
				else
					return {iter -> second, true};	
			}

			static struct ReadsResponse doReadsRequestFromFile(const ReadsRequest& request){
				if(is_test)
					return testReadsRequestFromFile(request);
				else
					return realReadsRequestFromFile(request);
			}
			static struct ReadsResponse realReadsRequestFromFile(const ReadsRequest& request){
				struct ReadsResponse response;
				response.otid = request.tid;
				response.ocid = request.cid;
				response.idx = request.idx;
				response.reads.center = {{0,0},{0,0}};
				response.reads.idx = request.idx;
				files_guard.lock();
				auto iter = files.find(request.file);
				ReadsIndex* index;
				if(iter == files.end()){
					index = ::parse::contig::idx::create_reads_index(request.file);
					if(!index){
						response.valid = false;
						files_guard.unlock();
						return response;
					}
					files[request.file] = index;
				}else
					index = iter -> second;
				files_guard.unlock();
				auto meta_reads = index -> get({request.file, request.idx});
				if(meta_reads){
					//get
					response.valid = true;
					auto [offset, length] = *meta_reads;
					char* buf = new char[length + 1];
					reads_files_guard.lock();
					FILE* reads_file;
					auto iter = reads_files.find(request.file);
					if(iter == reads_files.end()){
						reads_file = fopen(request.file.c_str(), "rb");
						reads_files[request.file] = reads_file;
					}else
						reads_file = iter -> second;
					fseek(reads_file, offset, SEEK_SET);
					fgets(buf, length + 1, reads_file);
					reads_files_guard.unlock();
					response.reads.data = buf;
					delete[] buf;
				}else
					response.valid = false;
				return response;
			}

			std::vector<struct ReadsResponse> 
			MultiReadsRequestsFromFile(const std::vector<struct ReadsRequest>& requests){
				std::vector<struct ReadsResponse> responses;
				for(const auto& request:requests)
					responses.push_back(std::move(ReadsRequestFromFile(request)));
				return responses;
			}
		}

		namespace reads{
			static std::vector<std::string> fake_reads = {
				"TCCGTCGATCCGTAA",
				"CCGTAACTAAGCGA",
				"GTCGATCCGTAATCC",
				"GTCGATCCGTAAATC",
				"GTCGATCCGTAA",
				"GTCGATCCGTAA",
				"GTCGATCCGTAA",
				"TAGCATGGGTCGATC",
			};

			void init_test(){
				is_test = true;
			}
			void clear_test(){
				is_test = false;
			}
			//
			static struct ReadsResponse testReadsRequestFromFile(const ReadsRequest& request){
				// auto [id, valid] = lock_entry_lookup(request);
				// auto [tid, cid] = id;
				struct ReadsResponse response;
				// if(!valid){
				// 	response.valid = false;
				// 	return response;
				// }
				response.valid = true;
				response.idx = request.idx;
				response.otid = request.tid;
				response.ocid = request.cid;
				response.reads.idx = request.idx;
				response.reads.center = {{0,0},{0,0}};
				response.reads.data = fake_reads[request.idx];
				return response;
			}
		}
	}
}