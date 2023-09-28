//common types
#include <contig/types.h>
#include <reads/types.h>
#include <vector>
#pragma once
namespace parse{
	namespace contig{
		namespace engine{
			struct SContigTask{
				int tid;
				int cid;
				int depth;
				int idx;//-1 means initial task
				::parse::contig::contig::SContig scontig;
				std::vector<int> request_ids;
			};
		}
	}
}