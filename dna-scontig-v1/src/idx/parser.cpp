#include <idx/parser.h>
#include <cstdio>
namespace parse{
	namespace contig{
		namespace idx{
			/**
			file format:
			idx
			reads
			center1
			center2
			empty line
			*/
			ReadsIndex* create_reads_index(const std::string& path){
				FILE* file = fopen(path.c_str(), "rb");
				if(!file)
					return nullptr;
				char buf[4096];
				int buf_count = -1;
				size_t offset = -1;
				size_t buf_len = -1;

				//line cursor of current reads data
				int line = 0;

				//reads data
				int idx = 0;
				int reads_offset = 0;
				int reads_len = 0;
				auto tree = new ReadsIndex;
				while(true){
					if(!buf_len)
						//eof
						break;
					else if(offset == buf_len){
						//read new fragment
						buf_len = fread(buf, 1, 4096, file);
						offset = 0;
						buf_count++;
					}else{
						switch(line){
							case 0:
								//idx
								if(buf[offset] <= '9' && buf[offset] >= '0')
									idx = idx * 10 + buf[offset] - '0';
								break;
							case 1:
								//reads
								if(!reads_offset)
									reads_offset = buf_count * 4096 + offset;
								if(buf[offset] == 'A' || buf[offset] == 'T' || buf[offset] == 'C' || buf[offset] == 'G')
									reads_len++;
								break;
							case 2:
							case 3:
								//center
								break;
							case 4:
								if (buf[offset] == '\n') {
									tree->insert({ {path, idx},{reads_offset, reads_len} });
									idx = 0;
									reads_offset = 0;
									reads_len = 0;
								}
								break;
						}
						if(buf[offset] == '\n')
							line = (line + 1) % 5;
						offset++;
					}
				}
				fclose(file);
				return tree;
			}
			/**
			 file format:
			 idx-matches
			 ...
			 */
			MatchIndex* create_matches_index(const std::string& path){
				FILE* file = fopen(path.c_str(), "rb");
				if(!file)
					return nullptr;
				char buf[4096];
				size_t offset = -1;
				size_t buf_len = -1;
				int idx = 0;
				int num_cache = 0;
				std::vector<int> match;
				int state = 0;//0-lead reads, 1-match reads
				auto tree = new MatchIndex;
				while(true){
					if(!buf_len)
						//eof
						break;
					else if(offset == buf_len){
						//read new fragment
						buf_len = fread(buf, 1, 4096, file);
						offset = 0;
					}else{
						switch(state){
							case 0:
								if(buf[offset] >= '0' && buf[offset] <= '9')
									idx = idx * 10 + buf[offset] - '0';
								else
									state = 1;
								break;
							case 1:
								if(buf[offset] >= '0' && buf[offset] <= '9')
									num_cache = num_cache * 10 + buf[offset] - '0';
								else if(buf[offset] == '\n'){
									if(num_cache)
										match.push_back(num_cache);
									tree -> insert({{path, idx}, match});
									match.clear();
									idx = 0;
									num_cache = 0;
									state = 0;
								}else{
									if(num_cache)
										match.push_back(num_cache);
									num_cache = 0;
								}
						}
						offset++;
					}
				}
				fclose(file);
				return tree;
			}
		}
	}
}