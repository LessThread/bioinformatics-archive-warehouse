/**
 * @brief      reads match and parsing
 * @author     Ji Xiangyu
 * @date       2022.8.18
 */
#pragma once
#include "types.h"
namespace parse{
	namespace contig{
		namespace reads{
			using Reads = struct Reads;
			//testify whether two reads is matched actually
			bool isMatch(const Reads& reads1, const Reads& reads2);
		}
	}
}