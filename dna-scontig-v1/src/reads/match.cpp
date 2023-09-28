#include <reads/match.h>
#include <cstring>
#include <vector>
#include <tuple>
namespace parse{
	namespace contig{
		namespace reads{
			//(read1 overlap, read2 overlap)
			static std::tuple<double, double> make_match(const std::string& read1, const std::string& read2);

			#define DEFAULT_OVERLAP 0.8
			bool isMatch(const Reads& reads1, const Reads& reads2){
				auto [suffix1, prefix2] = make_match(reads1.data, reads2.data);
				auto [suffix2, prefix1] = make_match(reads2.data, reads1.data);
				if(suffix1 >= DEFAULT_OVERLAP && prefix2 >= DEFAULT_OVERLAP)
					return true;
				if(suffix2 >= DEFAULT_OVERLAP && prefix1 >= DEFAULT_OVERLAP)
					return true;
				return false;
			}
			#undef DEFAULT_OVERLAP

			//read1 suffix vs. read2 prefix
			static std::tuple<double, double> make_match(const std::string& read1, const std::string& read2){
				std::vector<int> next;
				std::string tmp1 = read1;
				std::string tmp2 = read2;
				tmp1.append(1, 'N');
				tmp2.append(1, 'N');
				int len1 = tmp1.size();
				int len2 = tmp2.size();
				int offset1 = 0;
				int offset2 = -1;
				next.resize(len1);
				next[0] = -1;
				while(offset1 < len1 - 1){
					if(offset2 == -1 || tmp1[offset1] == tmp2[offset2])
						next[++offset1] = ++ offset2;
					else
						offset2 = next[offset2];
				}
				if (tmp1 == tmp2)
					return { 1,1 };
				return {(double)next[len1-1]/read1.size(), (double)next[len1-1]/read2.size()};
			}
		}
	}
}