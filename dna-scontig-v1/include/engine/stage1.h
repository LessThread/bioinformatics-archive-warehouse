/**
 * @brief      scontig-stage1, generate scontig
 * @author     Ji Xiangyu
 * @date       2022.8.18
 */
#pragma once
#include <string>
namespace parse{
	namespace contig{
		namespace engine{
			struct ScontigStage1Config{
				int thread_num;
				std::string reads_file;
				std::string match_file;
			};
			bool startSContigStage1(const struct ScontigStage1Config& config);
		}
	}
}