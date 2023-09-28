/**
 * @brief      Merge reads with reference
 * @author     Ji Xiangyu
 * @date       2022.8.20
 */
#pragma once
#include "types.h"
#include <vector>
namespace parse{
	namespace contig{
		namespace reads{
			struct Reads mergeFromReads(const std::vector<struct Reads>& reads, int reference);
			struct Reads mergeFromReads(const std::vector<struct Reads>& reads, struct Reads& reference);
			struct Reads mergeFromReads(const std::vector<struct Reads>& reads);
		}
	}
}