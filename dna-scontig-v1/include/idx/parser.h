//parse file to index
#pragma once
#include "tree.h"
#include <tuple>
#include <vector>
#include <string>
namespace parse{
	namespace contig{
		namespace idx{
			//<(file, idx), (offset, length)>
			using ReadsIndex = BPlusTree<std::tuple<std::string, int>, std::tuple<unsigned long, int>>;
			//<(file, idx), (matches)>
			using MatchIndex = BPlusTree<std::tuple<std::string, int>, std::vector<int>>;

			ReadsIndex* create_reads_index(const std::string& path);
			MatchIndex* create_matches_index(const std::string& path);
		}
	}
}