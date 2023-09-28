/**
 * @brief      intergrated header file.
 * @author     Ji Xiangyu
 * @date       2022.8.18
 */
#pragma once
#include<string>
namespace parse{
	namespace contig{
		struct SContigConfig{
			int thread_num;
			std::string reads_file;
			std::string match_file;
			std::string merge_file;//reserved
			std::string contig_file;//real output file
		};
		/**
		 * @brief      public API, cooked reads resides in reads_file, match table resides in match file. 
		 *
		 * @param[in]  config  The configuration
		 *
		 * @return     true if success, false if error.
		 * 
		 * @par output format
		 *     each contig end with an empty line
		 *     for each contig, record begins with its tag
		 *     the following lines are reads ids, scontig reference and merged reads sequencely.
		 *     ie., if a scontig with tag 2-3, and it is responsible for reads 132, 243,
		 *     as well as references to 1-2, 3-4, the output may be the following:
		 *     --start--
		 *     2-3
		 *     132 243 (end with a space)
		 *     1-2 3-4 (end with a space)
		 *     ATTCGGGTCA...(ie., merged reads, no space at the end of line)
		 *     (an empty line)
		 *     --end--
		 *     
		 */
		bool startSContig(const struct SContigConfig& config);
	}
}