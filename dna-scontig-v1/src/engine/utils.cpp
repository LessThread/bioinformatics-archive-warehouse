#include <engine/utils.h>
#include <reads/match.h>
namespace parse{
	namespace contig{
		namespace engine{
			std::vector<struct ::parse::contig::reads::ReadsResponse>
			get_next_batch(const std::string& file, const SContigTask& task){
				std::vector<::parse::contig::reads::ReadsRequest> requests;
				for(const auto& id:task.request_ids)
					requests.push_back({
						task.tid,
						task.cid,
						id,
						file
					});
				return MultiReadsRequestsFromFile(requests);
			}

			std::vector<int>
			mismatch_reads_filter(
				const struct ::parse::contig::reads::Reads& pattern,
				const std::vector<int>& id_list,
				const std::vector<struct ::parse::contig::reads::Reads>& targets
				){
				std::vector<int> ids;
				for(int i = 0;i < id_list.size();i++)
					if(isMatch(pattern, targets[i]))
						ids.push_back(id_list[i]);
				return ids;
			}
		}
	}
}