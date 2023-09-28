#include <engine/output.h>
#include <contig/storage.h>
#include <reads/io.h>
#include <reads/merge.h>
#include <fmt/core.h>
#include <thread>
#include <mutex>
#include <vector>
#include <tuple>
#include <cstdio>
#include <map>
#include <queue>
#include <algorithm>
namespace parse{
	namespace contig{
		namespace engine{
			static std::mutex lock;
			//for each thread_idx, merge and output to given file
			static void routine(const std::string& file, const std::string& reads_file, int thread_idx);
			static void print_with_lock(const std::string& file, 
										const std::tuple<int, int>& id,
										const std::vector<int>& reads,
										const std::vector<std::tuple<int, int>>& contigs,
										const std::string data);

			static std::map<std::string, FILE*> files;

			bool startMergeAndOutput(const std::string& file, const std::string& reads_file, int thread_num){
				fmt::print("start merge\n");
				//just in case
				files.clear();
				std::vector<std::thread> threads;
				for(int i = 0;i < thread_num;i++){
					std::thread t(routine, file, reads_file, i);
					threads.push_back(std::move(t));
				}
				for(int i = 0;i < thread_num;i++)
					threads[i].join();
				for(auto& [file, stream]:files)
					fclose(stream);
				files.clear();
				return true;
			}

			static void routine(const std::string& file, const std::string& reads_file, int thread_idx){
				auto& scontigs = ::parse::contig::contig::inspect_thread_local_scontigs(thread_idx);
				for(const auto& scontig:scontigs){
					std::vector<::parse::contig::reads::Reads> reads;
					std::vector<::parse::contig::reads::ReadsRequest> requests;
					std::vector<int> target_reads;
					std::vector<std::tuple<int, int>> scontig_read;
					std::queue<std::tuple<int, int>> pending_scontigs;
					pending_scontigs.push(scontig.id);
					while(!pending_scontigs.empty()){
						auto target_scontig_id = pending_scontigs.front();
						pending_scontigs.pop();
						auto target_scontig = ::parse::contig::contig::inspect_all_scontigs()[std::get<0>(target_scontig_id)][std::get<1>(target_scontig_id)];
						//reads is possibly duplicated, so just skip it instead of quit loop
						for(const auto& read:target_scontig.reads)
							if(std::find(target_reads.begin(), target_reads.end(), read) == target_reads.end())
								//not found, push back
								target_reads.push_back(read);
						for(const auto& id:target_scontig.contigs){
							if(std::find(scontig_read.begin(), scontig_read.end(), id) == scontig_read.end())
								pending_scontigs.push(id);
								scontig_read.push_back(id);
						}
					}
					//read reads finished, create request and merge
					for(const auto& read:target_reads)
						requests.push_back({
							std::get<0>(scontig.id),
							std::get<1>(scontig.id),
							read,
							reads_file
						});
					auto responses = ::parse::contig::reads::MultiReadsRequestsFromFile(requests);
					//process read response
					for(const auto& response:responses)
						if(response.valid)
							reads.push_back(response.reads);
					//merge reads
					auto merged_reads = ::parse::contig::reads::mergeFromReads(reads);
					print_with_lock(file, scontig.id, scontig.reads, scontig.contigs, merged_reads.data);
				}
			}

			static void print_with_lock(const std::string& file, 
										const std::tuple<int, int>& id,
										const std::vector<int>& reads,
										const std::vector<std::tuple<int, int>>& contigs,
										const std::string data){
				//thread safety of fmtlib is unknown, so lock print procedure just in case.
				std::lock_guard<std::mutex> guard(lock);
				auto iter = files.find(file);
				FILE* stream;
				if(iter == files.end()){
					//open and insert files, as file pointer is multiplexing, appending mode is not necessary.
					stream = fopen(file.c_str(), "wb");
					if(!stream)
						return;
					files.insert({file, stream});
				}else
					stream = iter -> second;

				//write with format
				fmt::print(stream, "{}-{}\n", std::get<0>(id), std::get<1>(id));
				for(const auto& read:reads)
					fmt::print(stream, "{} ", read);
				fmt::print(stream, "\n");
				for(const auto& [tid, cid]: contigs)
					fmt::print(stream, "{}-{} ", tid, cid);
				fmt::print(stream, "\n");
				fmt::print(stream, "{}\n", data);
				fmt::print(stream, "\n");
			}
		}
	}
}