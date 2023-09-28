#include <reads/merge.h>
#include <tuple>
namespace parse{
	namespace contig{
		namespace reads{
			struct Reads mergeFromReads(const std::vector<struct Reads>& reads){
				return mergeFromReads(reads, 0);
			}
			struct Reads mergeFromReads(const std::vector<struct Reads>& reads, int reference){
				std::vector<struct Reads> target_reads = reads;
				struct Reads refer = reads[reference];
				int i = 0;
				for(auto iter = target_reads.begin();iter != target_reads.end();)
					if(i == reference){
						iter = target_reads.erase(iter);
						break;
					}else{
						iter++;
						i++;
					}
				return mergeFromReads(target_reads, refer);
			}

			static constexpr int getmapping(char c);
			static constexpr char demapping(int m);
			//strict match
			static int ScoreAndOutput(const struct Reads& refer, const struct Reads& target);

			struct Reads mergeFromReads(const std::vector<struct Reads>& reads, struct Reads& reference){
				struct Reads result;
				std::vector<std::vector<int>> match_table;//(reads_id, reads_data)

				match_table.resize(reads.size() + 1);

				std::vector<int> refer_reads;
				for(const auto& seat:reference.data)
					refer_reads.push_back(getmapping(seat));
				match_table.push_back(refer_reads);

				int target_idx = 1;
				for(const auto& read:reads){
					auto remain = ScoreAndOutput(reference, read);
					for(int j = 0;j < remain;j++)
						match_table[target_idx].push_back(getmapping('N'));
					for(int j = 0;j < read.data.size();j++)
						match_table[target_idx].push_back(getmapping(read.data[j]));
					target_idx++;
				}

				int max = match_table[0].size();
				for(int i = 1;i < match_table.size();i++)
					if(match_table[i].size() > max)
						max = match_table[i].size();
				//match_table is ready, install merged data
				//pre-fill data
				result.data.append(max, 'N');
				for(int i = 0;i < max;i++){
					int candidate = -1;
					int count = 0;
					for(const auto& match_reads:match_table){
						if(i >= match_reads.size())
							continue;
						if (match_reads[i] == getmapping('N'))
							continue;
						if(!count) {
							candidate = match_reads[i];
							count = 1;
						}else if(candidate == match_reads[i])
							count++;
						else
							count--;
					}
					result.data[i] = demapping(candidate);
				}
				return result;
			}

			//TODO:can be replaces by KMP pre-process routine
			static int ScoreAndOutput(const struct Reads& refer, const struct Reads& target){
				int remain_max = -1;
				int remain_idx = -1;
				for (int i = 0; i < refer.data.size();i++) {
					int matches = 0;
					if(remain_max >= refer.data.size() - i)
						break;
					for(int j = i; j < refer.data.size();j++)
						if(refer.data[j] == target.data[j - i])
							matches++;
					if(remain_max > matches){
						remain_max = matches;
						remain_idx = i;
					}
				}
				return remain_idx;
			}

			static constexpr int getmapping(char c){
				switch(c){
					case 'A':
						return 0;
					case 'T':
						return 1;
					case 'C':
						return 2;
					case 'G':
						return 3;
					case 'N':
						return 4;
				}
				return -1;
			}
			static constexpr char demapping(int m){
				switch(m){
					case 0:
						return 'A';
					case 1:
						return 'T';
					case 2:
						return 'C';
					case 3:
						return 'G';
				}
				return 'N';
			}
		}
	}
}