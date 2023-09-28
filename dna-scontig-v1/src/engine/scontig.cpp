#include <engine/scontig.h>
#include <engine/stage1.h>
#include <engine/stage2.h>
#include <engine/output.h>
#include <contig/storage.h>
#include <reads/io.h>
#include <contig/io.h>

namespace parse{
	namespace contig{
		bool startSContig(const struct SContigConfig& config){
			::parse::contig::contig::init(config.thread_num);
			auto ret = engine::startSContigStage1({
							config.thread_num,
							config.reads_file,
							config.match_file
						})
						&& engine::startSContigStage2({
							config.thread_num,
							config.reads_file,
							config.contig_file,
							config.merge_file
						})
						&& engine::startMergeAndOutput(
							config.contig_file,
							config.reads_file,
							config.thread_num
							);
			::parse::contig::reads::clear();
			::parse::contig::contig::clear();
			return ret;
		}
	}
}