/**
 * @brief      some common algorithm and routine
 * @author     Ji Xiangyu
 * @date       2022.8.18
 */
#include "types.h"
#include <reads/io.h>
#include <vector>
#pragma once
namespace parse{
	namespace contig{
		namespace engine{
			std::vector<struct ::parse::contig::reads::ReadsResponse>
				get_next_batch(const std::string& file, const SContigTask& task);
			//filter mismatched reads, return matched reads' ids
			std::vector<int>
				mismatch_reads_filter(
					const struct ::parse::contig::reads::Reads& pattern,
					const std::vector<int>& id_list,
					const std::vector<struct ::parse::contig::reads::Reads>& targets
					);
		}
	}
}