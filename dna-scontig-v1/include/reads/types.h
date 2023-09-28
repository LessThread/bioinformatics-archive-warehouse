//common types
#pragma once
#include <tuple>
#include <string>
namespace parse{
	namespace contig{
		namespace reads{
			//a cooked reads
			struct Reads{
				int idx;
				std::string data;
				std::tuple<
					std::tuple<double, double>,
					std::tuple<double, double>
					> center;
			};
		}
	}
}