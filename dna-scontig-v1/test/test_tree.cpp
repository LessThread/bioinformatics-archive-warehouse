#include <idx/tree.h>
#include <cassert>
constexpr auto TIMES = 200;
int main(){
	::parse::contig::idx::BPlusTree<int, int> tree;
	for(int i = 0;i < TIMES;i++)
		tree.insert({ i, i });
	for(int i = 0;i < TIMES;i++){
		auto data = tree.get(i);
		assert(data);
		assert(*data == i);
	}
	tree.clear();

	for(int i = TIMES - 1;i >= 0;i--)
		tree.insert({i, i});
	for(int i = TIMES - 1;i >= 0;i--){
		auto data = tree.get(i);
		assert(data);
		assert(*data == i);
	}
	::parse::contig::idx::BPlusTree t(tree);
	tree.clear();
	t.clear();
	return 0;
}