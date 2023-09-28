#include <engine/stage2.h>
#include <contig/storage.h>
#include <reads/io.h>
#include <tuple>
#include <fmt/core.h>
namespace parse{
	namespace contig{
		namespace engine{
			bool startSContigStage2(const struct SContigStage2Config& config){
				fmt::print("start stage2\n");
				for(int j = 0;j < config.thread_num;j++){
					auto& scontig = ::parse::contig::contig::inspect_thread_local_scontigs(j);
					for(int i = 0;i < scontig.size();i++){
						auto [tid, cid] = scontig[i].id;
						for(auto iter = scontig[i].reads.begin();iter != scontig[i].reads.end();){
							//check for duplicate
							auto response = ::parse::contig::reads::ReadsRequestFromFile({tid, cid, *iter, config.reads_file});
							if(std::tie(response.otid, response.ocid) != scontig[i].id){
								//has been read
								iter = scontig[i].reads.erase(iter);
								scontig[i].contigs.push_back(std::make_tuple(response.otid, response.ocid));
							}else
								iter++;
						}
					}
				}
				return true;
			}
		}
	}
}