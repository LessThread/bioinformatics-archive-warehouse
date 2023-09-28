//B+ Tree
#pragma once
#include <tuple>
#include <functional>
#include <optional>
#include <queue>
namespace parse{
	namespace contig{
		namespace idx{
			/**
			 * @brief      This class describes a B+ tree
			 *
			 * @tparam     _idx_Ty   type of index
			 * @tparam     _data_Ty  type of value
			 * @note       This is a 'static' tree, so removal is forbidden.
			 */
			template<typename _idx_Ty, typename _data_Ty>
			class BPlusTree{
				public:
					BPlusTree();
					explicit BPlusTree(int base);
					~BPlusTree();
					explicit BPlusTree(const BPlusTree& tree);
					explicit BPlusTree(BPlusTree&& tree) noexcept;
					BPlusTree& operator=(const BPlusTree& tree);
					BPlusTree& operator=(BPlusTree&& tree) noexcept;
					bool insert(const std::tuple<_idx_Ty, _data_Ty>& value);
					std::optional<_data_Ty> get(const _idx_Ty& idx);
					bool update(const std::tuple<_idx_Ty, _data_Ty>& value);
					void foreach(const std::function<_data_Ty>& predicate);
					int size();
					void clear();
				private:
					struct BPlusTreeNode{
						int bound_count;
						struct BPlusTreeNode** nodes;
						_idx_Ty* bounds;
						_data_Ty* data;
						bool is_leaf;
						struct BPlusTreeNode* sibling;
						struct BPlusTreeNode* parent;
					};
					using Node = struct BPlusTreeNode;
					constexpr static int DEFAULT_LEVEL = (1<<12)/sizeof(Node);
					int count;
					int base;
					Node* root;
					Node* leaf_head;
					//insert new key into 'start' leaf node, split node if neccessary
					void split(Node* start, const std::tuple<_idx_Ty, _data_Ty>& value);
			};
		}
	}
}


//impl

namespace parse{
	namespace contig{
		namespace idx{

			template<typename _idx_Ty, typename _data_Ty>
			BPlusTree<_idx_Ty, _data_Ty>::BPlusTree():BPlusTree(DEFAULT_LEVEL){}
			template<typename _idx_Ty, typename _data_Ty>
			BPlusTree<_idx_Ty, _data_Ty>::BPlusTree(int base):count(0), base(base), root(nullptr), leaf_head(nullptr){}

			template<typename _idx_Ty, typename _data_Ty>
			BPlusTree<_idx_Ty, _data_Ty>::~BPlusTree(){
				this -> clear();
			}

			template<typename _idx_Ty, typename _data_Ty>
			BPlusTree<_idx_Ty, _data_Ty>::BPlusTree(const BPlusTree& tree):BPlusTree() {
				auto cursor = tree.leaf_head;
				while(cursor){
					for(int i = 0;i < cursor -> bound_count;i++)
						this -> insert(std::make_tuple(cursor -> bounds[i], cursor -> data[i]));
					cursor = cursor -> sibling;
				}
			}
			template<typename _idx_Ty, typename _data_Ty>
			BPlusTree<_idx_Ty, _data_Ty>::BPlusTree(BPlusTree&& tree) noexcept{
				this -> root = tree.root;
				this -> leaf_head = tree.leaf_head;
				tree.root = nullptr;
				tree.leaf_head = nullptr; 
			}
			template<typename _idx_Ty, typename _data_Ty>
			BPlusTree<_idx_Ty, _data_Ty>& BPlusTree<_idx_Ty, _data_Ty>::operator=(const BPlusTree& tree){
				if(&tree == this);
					return *this;
				auto cursor = tree.leaf_head;
				while(cursor){
					for(int i = 0;i < cursor -> bound_count;i++)
						this -> insert(std::make_tuple(cursor -> bounds[i], cursor -> data[i]));
					cursor = cursor -> sibling;
				}
				return *this;
			}
			template<typename _idx_Ty, typename _data_Ty>
			BPlusTree<_idx_Ty, _data_Ty>& BPlusTree<_idx_Ty, _data_Ty>::operator=(BPlusTree&& tree) noexcept{
				if(&tree == this)
					return *this;
				this -> root = tree.root;
				this -> leaf_head = tree.leaf_head;
				tree.root = nullptr;
				tree.leaf_head = nullptr;
				return *this;
			}

			template<typename _idx_Ty, typename _data_Ty>
			bool BPlusTree<_idx_Ty, _data_Ty>::insert(const std::tuple<_idx_Ty, _data_Ty>& value){
				if(!root){
					//empty, just insert
					//node0:root node
					auto node = new struct BPlusTreeNode;
					node -> bound_count = 1;
					node -> is_leaf = false;
					node -> nodes = new struct BPlusTreeNode*[this -> base + 2]();
					node -> bounds = new _idx_Ty[this -> base + 1];
					node -> data = nullptr;
					node -> sibling = nullptr;
					node -> parent = nullptr;
					//node1:leaf node
					auto leaf = new struct BPlusTreeNode;
					leaf -> bound_count = 1;
					leaf -> is_leaf = true;
					leaf -> nodes = nullptr;
					leaf -> sibling = nullptr;
					leaf -> bounds = new _idx_Ty[this -> base + 1];
					leaf -> data = new _data_Ty[this -> base + 1];
					leaf -> parent = node;
					//install nodes
					root = node;
					node -> bounds[0] = std::get<0>(value);
					leaf -> bounds[0] = std::get<0>(value);
					node -> nodes[1] = leaf;
					leaf -> data[0] = std::get<1>(value);
					this -> count += 1;
					this -> leaf_head = leaf;
					return true;
				}
				auto node = root;
				while(!node -> is_leaf){
					int i;
					for(i = 0;i < node -> bound_count;i++){
						if(node -> bounds[i] == std::get<0>(value))
							return false;
						if(node -> bounds[i] > std::get<0>(value))
							break;
					}
					if(!node -> nodes[i]){
						auto new_leaf = new struct BPlusTreeNode;
						new_leaf -> bound_count = 0;
						new_leaf -> nodes = nullptr;
						new_leaf -> is_leaf = true;
						new_leaf -> bounds = new _idx_Ty[this -> base + 1];
						new_leaf -> data = new _data_Ty[this -> base + 1];
						new_leaf -> parent = node;
						//process sibling, as the most left leaf may update leaf_head
						if(std::get<0>(value) < this -> leaf_head -> bounds[0]){
							new_leaf -> sibling = this ->leaf_head;
							this -> leaf_head = new_leaf;
						}else
							new_leaf -> sibling = nullptr;
						node -> nodes[i] = new_leaf;
					}
					node = node -> nodes[i];
				}
				//leaf node
				for(int i = 0;i < node -> bound_count;i++){
					if(node -> bounds[i] == std::get<0>(value))
						return false;
				}
				this -> split(node, value);
				this -> count += 1;
				return true;
			}

			template<typename _idx_Ty, typename _data_Ty>
			std::optional<_data_Ty> BPlusTree<_idx_Ty, _data_Ty>::get(const _idx_Ty& idx){
				auto node = root;
				if(!node)
					return std::nullopt;
				while(!node -> is_leaf){
					int i;
					for(i = 0;i < node -> bound_count;i++)
						if(node -> bounds[i] > idx)
							break;
					if(!node -> nodes[i])
						return std::nullopt;
					node = node -> nodes[i];
				}
				for(int i = 0;i < node -> bound_count;i++)
					if(node -> bounds[i] == idx)
						return node -> data[i];
				return std::nullopt;
			}

			template<typename _idx_Ty, typename _data_Ty>
			bool BPlusTree<_idx_Ty, _data_Ty>::update(const std::tuple<_idx_Ty, _data_Ty>& value){
				const auto& [idx, data] = value;
				auto node = root;
				if(!node)
					return false;
				while(!node -> is_leaf){
					int i;
					for(i = 0;i < node -> bound_count;i++)
						if(node -> bounds[i] > idx)
							break;
					node = node -> nodes[i];
				}
				for(int i = 0;i < node -> bound_count;i++)
					if(node -> bounds[i] == idx){
						node -> data[i] = data;
						return true;
					}
				return false;
			}

			template<typename _idx_Ty, typename _data_Ty>
			void BPlusTree<_idx_Ty, _data_Ty>::foreach(const std::function<_data_Ty>& predicate){
				auto cursor = tree.leaf_head;
				while(cursor){
					for(int i = 0;i < bound_count;i++)
						predicate(cursor -> data[i]);
					cursor = cursor -> sibling;
				}
			}

			template<typename _idx_Ty, typename _data_Ty>
			int BPlusTree<_idx_Ty, _data_Ty>::size(){
				return this -> count;
			}

			template<typename _idx_Ty, typename _data_Ty>
			void BPlusTree<_idx_Ty, _data_Ty>::clear(){
				if (!root)
					return;
				std::queue<struct BPlusTreeNode*> pending_clear;
				pending_clear.push(root);
				while(!pending_clear.empty()){
					auto node = pending_clear.front();
					pending_clear.pop();
					if(!node -> is_leaf){
						for(int i = 0;i < node -> bound_count + 1;i++)
							if(node -> nodes[i])
								pending_clear.push(node -> nodes[i]);
						delete[] node -> nodes;
					}else
						delete[] node -> data;
					delete[] node -> bounds;
					delete node;
				}
				this -> count = 0;
				this -> root = nullptr;
				this -> leaf_head = nullptr;
			}

			template<typename _idx_Ty, typename _data_Ty>
			void BPlusTree<_idx_Ty, _data_Ty>::split(Node* start, const std::tuple<_idx_Ty, _data_Ty>& value){
				//p1:insert into leaf node forcefully, fix it later
				int insert_point = 0;
				for(;insert_point < start -> bound_count;insert_point++)
					if(start -> bounds[insert_point] > std::get<0>(value))
						break;
				//leaf node
				for(int j = start -> bound_count;j > insert_point;j--){
					start -> bounds[j] = start -> bounds[j-1];
					start -> data[j] = start -> data[j-1];
				}
				start -> bounds[insert_point] = std::get<0>(value);
				start -> data[insert_point] = std::get<1>(value);
				start -> bound_count++;
				//NOTE:base is temporily broken, but we would fix it later
				//out of bound, so split it
				//promote idx
				_idx_Ty next_idx;
				//target parent node
				auto target_parent = start -> parent;
				//new_node(right child)
				struct BPlusTreeNode* new_node;
				if(start -> bound_count == this -> base + 1){
					//[0, splitpoint) to start
					//[splitpoint, this -> base] to new_node
					int splitpoint = (this -> base + 1) >> 1;
					next_idx = start -> bounds[splitpoint];
					new_node = new struct BPlusTreeNode;
					new_node -> is_leaf = true;
					new_node -> sibling = start -> sibling;
					start -> sibling = new_node;
					new_node -> bound_count = this -> base - splitpoint + 1;
					new_node -> bounds = new _idx_Ty[this -> base + 1];
					new_node -> parent = start -> parent;
					new_node -> nodes = nullptr;
					new_node -> data = new _data_Ty[this -> base + 1];
					for(int i = splitpoint;i <= this -> base;i++){
						new_node -> bounds[i - splitpoint] = start -> bounds[i];
						new_node -> data[i - splitpoint] = start -> data[i];
					}
					start -> bound_count = splitpoint;
				}else
					return;
				//at least target_parent is not nullptr at the beginning
				while(target_parent){
					//p2-1:insert first
					for(insert_point = 0;insert_point < target_parent -> bound_count;insert_point++)
						if(target_parent -> bounds[insert_point] > next_idx)
							break;
					//non-leaf node
					for(int j = target_parent -> bound_count;j > insert_point;j--){
						target_parent -> bounds[j] = target_parent -> bounds[j-1];
						target_parent -> nodes[j+1] = target_parent -> nodes[j];
					}
					target_parent -> bounds[insert_point] = next_idx;
					target_parent -> nodes[insert_point+1] = new_node;
					target_parent -> bound_count++;

					//p2-2:process split
					if(target_parent -> bound_count == this -> base + 1){
						int splitpoint = (this -> base + 1) >> 1;
						//split non-leaf root
						//[0, splitpoint) to target_parent
						//[splitpoint, this -> base] to new_inode
						next_idx = target_parent -> bounds[splitpoint];
						auto new_inode = new struct BPlusTreeNode;
						new_inode -> is_leaf = false;
						new_inode -> sibling = nullptr;
						new_inode -> bound_count = this -> base - splitpoint + 1;
						new_inode -> bounds = new _idx_Ty[this -> base + 1];
						new_inode -> parent = target_parent -> parent;
						new_inode -> nodes = new struct BPlusTreeNode*[this -> base + 2]();
						new_inode -> data = nullptr;
						for(int i = splitpoint;i <= this -> base;i++){
							new_inode -> bounds[i - splitpoint] = target_parent -> bounds[i];
							new_inode -> nodes[i - splitpoint] = target_parent -> nodes[i];
						}
						new_inode -> nodes[this -> base - splitpoint + 1] = target_parent -> nodes[this -> base + 1];
						new_inode -> nodes[0] = nullptr; 
						target_parent -> bound_count = splitpoint;
						new_node = new_inode;
						target_parent = target_parent -> parent;
					}else
						return;
				}
				//root is full, associate this -> root and new_node with new root
				auto new_root = new struct BPlusTreeNode;
				new_root -> bound_count = 1;
				new_root -> nodes = new struct BPlusTreeNode*[this -> base + 2];
				new_root -> bounds = new _idx_Ty[this -> base + 1];
				new_root -> data = nullptr;
				new_root -> is_leaf = false;
				new_root -> sibling = nullptr;
				new_root -> parent = nullptr;
				new_root -> bounds[0] = next_idx;
				new_root -> nodes[0] = this -> root;
				new_root -> nodes[1] = new_node;
				new_node -> parent = new_root;
				this->root->parent = new_root;
				this -> root = new_root;
			}
		}
	}
}