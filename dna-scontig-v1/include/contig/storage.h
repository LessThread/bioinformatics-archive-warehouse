/**
 * @brief      Contig pool, used to store raw contig
 * @author     Ji Xiangyu
 * @date       2022.8.12
 */
#pragma once
#include "types.h"
#include <map>
#include <vector>
namespace parse{
	namespace contig{
		namespace contig{
			void init(int thread_num);
			void insert_scontig(int tid, const SContig& scontig);
			void insert_scontig(int tid, SContig&& scontig);
			std::vector<SContig>& inspect_thread_local_scontigs(int tid);
			std::vector<std::vector<SContig>>& inspect_all_scontigs();
		}
	}
}