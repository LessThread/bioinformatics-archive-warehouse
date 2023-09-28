#include <engine/stage1.h>
#include <engine/types.h>
#include <engine/utils.h>
#include <string>
#include <queue>
#include <thread>
#include <vector>
#include <reads/io.h>
#include <tuple>
#include <contig/io.h>
#include <contig/storage.h>
#include <fmt/core.h>
namespace parse{
	namespace contig{
		namespace engine{
			struct ThreadEnv{
				int tid;
				int group_num;
				std::string reads_file;
				std::string match_file;
			};

			static void routine(const struct ThreadEnv& env);
			static int get_line_limits(const std::string& file);

			bool startSContigStage1(const struct ScontigStage1Config& config){
				fmt::print("start stage1\n");
				std::vector<std::thread> threads;
				for(int i = 0;i < config.thread_num;i++){
					struct ThreadEnv env = {i, config.thread_num, config.reads_file, config.match_file};
					std::thread t(routine, std::move(env));
					threads.push_back(std::move(t));
				}
				for(int i = 0;i < config.thread_num;i++)
					threads[i].join();
				return true;
			}

			using SContig = struct ::parse::contig::contig::SContig;
			static void routine(const struct ThreadEnv& env){
				//thread routine
				//p1:for each thread, fetch its reads
				
				//p2:for each reads, fetch match entry

				//p2-1:if any of entry has been processed, hang up(submit) current scontig

				//p3:for each match entry, running bimatch, filter mismatched reads

				//p4:for each matched reads, repeat p2-p4(may used BFS instead of DFS to avoid stackoverflow)

				//p1-impl:original responsibility:tid+group*k,k=0,1,2,...
				int tid = env.tid;
				int group = env.group_num;
				int limit = get_line_limits(env.match_file);
				for(int cid = 0;group*cid+tid < limit;cid++){
					fmt::print("thread:#{}, process:{}/{}\n", tid, cid, (limit - tid)/group);
					SContig scontig;
					scontig.id = {tid, cid};
					scontig.contigs = {};
					scontig.reads = {};
					std::queue<SContigTask> tasks;
					tasks.push({
						tid,
						cid,
						1,
						-1,
						{{tid, cid}, {}, {}},
						{group * cid + tid + 1}
					});
					while(!tasks.empty()){
						auto task = tasks.front();
						tasks.pop();
						std::vector<int> next_batch;
						if(task.idx == -1){
							//initial task, skip match routine
							//but ownership must be checked
							auto response = ::parse::contig::reads::ReadsRequestFromFile({
								tid,
								cid,
								task.request_ids[0],
								env.reads_file
							});
							if(!response.valid||std::tie(response.otid, response.ocid) != std::make_tuple(tid, cid))
								break;
							//push back
							next_batch.push_back(task.request_ids[0]);
						}else{
							//not initial task, need to match and filter
							std::vector<::parse::contig::reads::ReadsResponse> valid_responses;
							//check target ownership
							auto target_responses = get_next_batch(env.reads_file, task);
							for(auto response:target_responses){
								//it is clearly valid, or match file is wrong, except that reads is read by the same tag
								if(!response.valid)
									//read by the same tag
									//skip it
									continue;
								else if(std::tie(response.otid, response.ocid) == std::make_tuple(tid, cid)){
									//owned by current tag
									//so we can continue
									valid_responses.push_back(std::move(response));
								}else{
									//not owned by current tag, so push back contigs
									scontig.contigs.push_back({response.otid, response.ocid});
								}
							}
							
							//read pattern reads
							auto pattern_response = ::parse::contig::reads::ReadsRequestFromFile({
								tid,
								cid,
								task.idx,
								env.reads_file
							});
							//it's sure that it is valid and owned by current tag
							auto pattern = pattern_response.reads;

							//now pattern resides in 'pattern'
							//target resides in 'valid_response'
							//it's time to match!
							std::vector<struct ::parse::contig::reads::Reads> target_reads;
							std::vector<int> id_lists;
							for(auto response:valid_responses){
								target_reads.push_back(std::move(response.reads));
								id_lists.push_back(std::move(response.idx));
							}
							next_batch = std::move(mismatch_reads_filter(pattern, id_lists, target_reads));
						}
						//now next_batch stores next pending reads, so record it and generate another task
						for(const auto& pending_read:next_batch){
							//s1:for each pending reads, check ownership.
							//if it is cooked, record tag instead
							//it is clear that reads is valid, or match file is wrong, except unrepeatable read
							auto match_response = ::parse::contig::contig::MatchRequestFromFile({
								tid,
								cid,
								pending_read,
								env.match_file
							});
							if(!match_response.valid)
								//repeatedly read, skip it
								continue;
							else if(std::tie(match_response.tid,match_response.cid) != std::tie(tid, cid)){
								//not owned by current tag, record it
								scontig.contigs.push_back({match_response.tid, match_response.cid});
							}else{
								//owned by current tag
								//s2:record to scontig
								scontig.reads.push_back(pending_read);
								//now next pending-related reads resides in match_reponse.entry, generate task now!
								SContigTask next_task = {
									tid,
									cid,
									task.depth + 1,
									pending_read,
									scontig,
									std::move(std::get<1>(match_response.entry))
								};
								tasks.push(std::move(next_task));
							}
						}
					}
					if(scontig.contigs.size()||scontig.reads.size())
						::parse::contig::contig::insert_scontig(tid, scontig);
				}
			}
			static int get_line_limits(const std::string& file){
				return ::parse::contig::contig::get_file_limits(file);
			}
		}
	}
}