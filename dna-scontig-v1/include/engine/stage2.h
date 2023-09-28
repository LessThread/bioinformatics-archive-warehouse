/**
 * @brief      scontig stage2, optimize and merge
 * @author     Ji Xiangyu
 * @date       2022.8.18
 */
#pragma once
#include <string>
namespace parse{
	namespace contig{
		namespace engine{
			struct SContigStage2Config{
				int thread_num;
				std::string reads_file;
				std::string contig_file;
				std::string merge_file;
			};
			bool startSContigStage2(const struct SContigStage2Config& config);
		}
	}
}