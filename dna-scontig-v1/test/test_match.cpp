#include <fmt/core.h>
#include <reads/io.h>
#include <contig/io.h>
#include <contig/storage.h>
#include <engine/scontig.h>
#include <cassert>

#define TEST_ROOT "D:\\service\\project\\tmp"

int main(int argc, char** argv){
	//::parse::contig::reads::init_test();
	//::parse::contig::contig::init_test();
	::parse::contig::startSContig({
		8,
		TEST_ROOT"/reads.txt",
		TEST_ROOT"/match.txt",
		"",
		TEST_ROOT"/output.txt"
	});
	auto& scontigs = ::parse::contig::contig::inspect_all_scontigs();
	for(const auto scontig_data:scontigs){
		for(const auto& scontig:scontig_data){
			//print contig1
			auto [tid, cid] = scontig.id;
			fmt::print("tag:{}-{}\n", tid, cid);
			fmt::print("\treads:\n");
			for(const auto& reads:scontig.reads)
				fmt::print("\t\t{}\n", reads);
			fmt::print("\tcontigs:\n");
			for(const auto& [tid, cid]:scontig.contigs)
				fmt::print("\t\t{}-{}\n", tid, cid);
		}
	}
	//::parse::contig::reads::clear_test();
	//::parse::contig::contig::clear_test();
	return 0;
}