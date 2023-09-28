#include <contig/storage.h>
#include <vector>
#include <mutex>
namespace parse{
	namespace contig{
		namespace contig{
			using SContigPool = std::vector<std::vector<SContig>>;
			static SContigPool pool;
			static int threads_num;
			void init(int thread_num){
				for(int i = 0;i < thread_num;i++)
					pool.push_back(std::vector<SContig>());
			}
			void insert_scontig(int tid, const SContig& scontig){
				//thread has its local storage, so no lock is needed.
				pool[tid].push_back(scontig);
			}
			void insert_scontig(int tid, SContig&& scontig){
				pool[tid].push_back(std::move(scontig));
			}
			std::vector<SContig>& inspect_thread_local_scontigs(int tid){
				return pool[tid];
			}
			std::vector<std::vector<SContig>>& inspect_all_scontigs(){
				return pool;
			}
		}
	}
}