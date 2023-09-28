#pragma once
#include <vector>
#include <tuple>
namespace parse{
	namespace contig{
		namespace contig{
			//<leadid, (matched id...)>
			using MatchEntry = std::tuple<int, std::vector<int>>;
			using SContigId = std::tuple<int, int>;//(tid, cid)
			struct SContig{
				SContigId id;
				std::vector<SContigId> contigs;
				std::vector<int> reads;
			};
		}
	}
}