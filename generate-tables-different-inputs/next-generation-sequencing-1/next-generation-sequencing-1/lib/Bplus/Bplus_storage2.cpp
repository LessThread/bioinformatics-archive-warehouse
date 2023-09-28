#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <vector>
#include <io.h>
#include <process.h>
#include <list>
#include <mutex>
#include <string.h>

// #include <memory>
// #include <iterator>
#include <algorithm>
// #include <cstdio>
// #include <thread>
// #include <list>
// #include <map>
// #include <functional>
// #include <future>
// #include <condition_variable>
// #include <cmath>
// #include <utility>
// #include <queue>
// #include <windows.h>
// #include <chrono>

#ifndef INVALID_OFFSET
#define INVALID_OFFSET 0xdeadbeef
#endif

#define key_t int
#define stringlength 500
#define MAX_CACHE_NUM 20
// #define readlength 5
using namespace std;

static int _max_order;
static int _min_order;
static int _max_entry;
static int _min_entry;

// static ifstream filein;
// static std::mutex mymutex;
#define is_leaf 1
#define non_leaf 0
#define free -1
#define magicnum 

class BplusNode {
public:
    off_t self;
    off_t parent;
    off_t next;
    off_t prev;

    int type;

    std::vector<key_t> keys;
    std::vector<off_t> children;
    std::vector<std::string> data;

    BplusNode() {
        this->self = INVALID_OFFSET;
        this->parent = INVALID_OFFSET;
        this->prev = INVALID_OFFSET;
        this->next = INVALID_OFFSET;
        keys.clear();
        children.clear();
        data.clear();
    }
    ~BplusNode() {
        keys.clear();
        children.clear();
        data.clear();
    }
};

static std::pair<bool, BplusNode> used[MAX_CACHE_NUM];

class BplusTree {
private:
    int level;
    off_t file_size;
    off_t root;
    std::string filepath;
    std::list<off_t> free_blocks;
    std::mutex mymutex;
    int _block_size;

    inline BplusNode* node_insert_cache(off_t offset);
    inline BplusNode* node_seek(off_t offset);
    void node_delete(BplusNode* node, BplusNode* left, BplusNode* right);
    inline void node_write_and_pop(BplusNode* node);
    off_t new_node_append(BplusNode* node);
    inline void child_node_write_and_pop(BplusNode* parent, off_t child_offset);
    inline void parent_key_update(BplusNode* node, key_t key);
    void node_add(BplusNode* node, BplusNode* left);
    bool parent_node_build(BplusNode* l_ch, BplusNode* r_ch, key_t key1, key_t key2);
    std::pair<key_t, key_t> non_leaf_split(BplusNode* node, BplusNode* left, BplusNode* child, key_t key, int insert);
    void non_leaf_shift_element_insert(BplusNode* right, BplusNode* left, BplusNode* parent, int parent_left_index, int remove, bool from, key_t key, BplusNode* child);
    void non_leaf_simple_insert(BplusNode* node, BplusNode* child, key_t key, int insert);
    bool non_leaf_insert(BplusNode* node, BplusNode* child, key_t key);
    std::pair<key_t, key_t> leaf_split(BplusNode* leaf, BplusNode* left, key_t key, std::string datastring, int insert);
    void leaf_shift_element_insert(BplusNode* right, BplusNode* left, BplusNode* parent, int parent_left_index, int remove, bool from, key_t key, std::string datastring);
    void leaf_simple_insert(BplusNode* leaf, key_t key, std::string datastring, int insert);
    bool leaf_insert(BplusNode* leaf, key_t key, std::string datastring);
    void non_leaf_shift_element_remove(BplusNode* right, BplusNode* left, BplusNode* parent, int parent_left_index, int remove, bool from);
    void non_leaf_merge(BplusNode* right, BplusNode* left, BplusNode* parent, int parent_right_index, int remove, bool from);
    inline void non_leaf_simple_remove(BplusNode* node, int remove);
    void non_leaf_remove(BplusNode* node, int remove);
    void leaf_shift_element_remove(BplusNode* right, BplusNode* left, BplusNode* parent, int parent_left_index, int remove, bool from);
    void leaf_merge(BplusNode* right, BplusNode* left, BplusNode* parent, int parent_right_index, int remove, bool from);
    void leaf_simple_remove(BplusNode* leaf, int remove);
    bool leaf_remove(BplusNode* leaf, key_t key);
    void WriteDisk(BplusNode* node);
    void ReadDisk(BplusNode* node, off_t offset);

public:
    bool BplusTree_insert(key_t key, std::string datastring);
    bool BplusTree_delete(key_t key);
    std::string BplusTree_search(key_t key);
    std::string BplusTree_search_range(key_t key1, key_t key2);
    BplusTree(std::string Filepath);
    ~BplusTree();
    bool BplusTree_Init(std::string Filepath, int block_size = 16000);
    void BplusTree_Deinit();
};

static int key_binary_search_search(BplusNode* node, key_t target);
static int key_binary_search_remove(BplusNode* node, key_t target);
static int key_binary_search_non_leaf_insert(BplusNode* node, key_t target);

// 在内存中加入
static inline BplusNode* cache_refer() {
    for (int i = 0; i < MAX_CACHE_NUM; i++) {
        if (!used[i].first) {
            used[i].first = true;
            BplusNode node;
            used[i].second = node;
            return &used[i].second;
        }
    }
    return nullptr;
}

// 即在内存中删除node
static inline void cache_defer(BplusNode* node) {
    if (node == nullptr)
        return;
    for (int i = 0; i < MAX_CACHE_NUM; i++) {
        if (&used[i].second == node) {
            used[i].first = false;
            return;
        }
    }
}

// 内存中创建新的非叶node
static inline BplusNode* non_leaf_new() {
    BplusNode* node = cache_refer();
    node->type = non_leaf;
    node->keys.reserve(_max_order + 1);
    node->children.reserve(_max_order + 1);
    return node;
}

// 内存中创建新的叶node
static inline BplusNode* leaf_new() {
    BplusNode* node = cache_refer();
    node->type = is_leaf;
    node->keys.reserve(_max_entry + 1);
    node->data.reserve(_max_entry + 1);
    return node;
}

static inline bool sibling_more_select(BplusNode* l_sib, BplusNode* r_sib, BplusNode* parent, int i) {
    if (i == 0)
        return true; // true为右兄弟
    else if (i == parent->keys.size() - 1)
        return false; // false为左兄弟
    else
        return l_sib->keys.size() >= r_sib->keys.size() ? false : true; // false为左兄弟，true为右兄弟
}

static inline bool sibling_less_select(BplusNode* l_sib, BplusNode* r_sib, BplusNode* parent, int i) {
    if (i == 0)
        return true; // true为右兄弟
    else if (i == parent->keys.size() - 1)
        return false; // false为左兄弟
    else
        return l_sib->keys.size() <= r_sib->keys.size() ? false : true; // false为左兄弟，true为右兄弟
}

// 将所读取的偏移量的值插入内存
inline BplusNode* BplusTree::node_insert_cache(off_t offset) {
    if (offset == INVALID_OFFSET)
        return nullptr;
    BplusNode* node = cache_refer();
    this->ReadDisk(node, offset);
    return node;
}

// 将offset指向的结点存入内存中，返回其所在内存的地址，但不标记已经使用过used，即只使用一次
inline BplusNode* BplusTree::node_seek(off_t offset) {
    if (offset == INVALID_OFFSET)
        return nullptr;
    int i;
    for (i = 0; i < MAX_CACHE_NUM; i++) {
        if (!used[i].first) {
            BplusNode node;
            used[i].second = node;
            this->ReadDisk(&used[i].second, offset);
            return &used[i].second;
        }
    }
    return nullptr;
}

// 将结点从内存中取出，并且写入文件，内存删除结点
inline void BplusTree::node_write_and_pop(BplusNode* node) {
    if (node != nullptr) {
        this->WriteDisk(node);
        cache_defer(node);
    }
}

// 即在磁盘逻辑上使node加入，如果空闲块为空，就让node加在文件的尾部，需要进行一次node_write_and_pop
off_t BplusTree::new_node_append(BplusNode* node) {
    if (this->free_blocks.empty()) {
        node->self = this->file_size;
        this->file_size += this->_block_size;
    }
    else {
        off_t block;
        block = this->free_blocks.front();
        this->free_blocks.pop_front();
        if (!this->free_blocks.empty()) {
            BplusNode* right_freeblock = this->node_insert_cache(this->free_blocks.front());
            right_freeblock->prev = INVALID_OFFSET;
            this->node_write_and_pop(right_freeblock);
        }
        node->self = block;
    }
    return node->self;
}

// 将孩子的父母偏移量更新，前提是child_offset对应的child不在内存中
inline void BplusTree::child_node_write_and_pop(BplusNode* parent, off_t child_offset) {
    BplusNode* child = this->node_insert_cache(child_offset);
    child->parent = parent->self;
    this->node_write_and_pop(child);
}

// 这个parent_key_update只有在确定key是node的keys的最后结点
inline void BplusTree::parent_key_update(BplusNode* node, key_t key) {
    BplusNode* tempnode = node;
    if (tempnode->parent == INVALID_OFFSET)
        return;
    int keysearch = tempnode->keys.front();
    tempnode = this->node_seek(tempnode->parent);
    int position = key_binary_search_search(tempnode, keysearch);
    while (position == tempnode->keys.size() - 1 && tempnode->parent != INVALID_OFFSET) {
        tempnode->keys[position] = key;
        this->WriteDisk(tempnode);
        tempnode = this->node_seek(tempnode->parent);
        position = key_binary_search_search(tempnode, keysearch);
    }
    tempnode->keys[position] = key;
    this->WriteDisk(tempnode);
}

// 磁盘逻辑上将left增加为node的左节点，left已经在内存中存在的情况下，但没有对left和node进行写入
void BplusTree::node_add(BplusNode* node, BplusNode* left) {
    this->new_node_append(left);
    BplusNode* prev = this->node_insert_cache(node->prev);
    if (prev != nullptr) {
        prev->next = left->self;
        left->prev = prev->self;
        this->node_write_and_pop(prev);
    }
    else
        left->prev = INVALID_OFFSET;
    left->next = node->self;
    node->prev = left->self;
}

bool BplusTree::parent_node_build(BplusNode* l_ch, BplusNode* r_ch, key_t key1, key_t key2) {
    if (r_ch->parent == INVALID_OFFSET) {
        BplusNode* parent = non_leaf_new();
        parent->keys.emplace_back(key1);
        parent->keys.emplace_back(key2);
        parent->children.emplace_back(l_ch->self);
        parent->children.emplace_back(r_ch->self);
        this->root = this->new_node_append(parent);
        l_ch->parent = parent->self;
        r_ch->parent = parent->self;
        this->level++;
        this->node_write_and_pop(l_ch);
        this->node_write_and_pop(r_ch);
        this->node_write_and_pop(parent);
        return true;
    }
    else {
        this->node_write_and_pop(r_ch);
        return this->non_leaf_insert(this->node_insert_cache(r_ch->parent), l_ch, key1);
    }
    return false;
}

// 当给非叶结点增加元素，当insert < _min_order，且结点满时，左增加结点，left是新创建的结点，没有在磁盘上写入，没有修改父母的
std::pair<key_t, key_t> BplusTree::non_leaf_split(BplusNode* node, BplusNode* left, BplusNode* child, key_t key, int insert) {
    this->node_add(node, left);

    node->keys.insert(node->keys.begin() + insert, key);
    node->children.insert(node->children.begin() + insert, child->self);

    if (insert == node->keys.size() - 1)
        this->parent_key_update(node, key);

    for (int i = 0; i < _min_order; i++) {
        left->keys.emplace_back(node->keys[i]);
        left->children.emplace_back(node->children[i]);
        if (i != insert)
            this->child_node_write_and_pop(left, left->children[i]);
    }

    if (insert >= _min_order)
        child->parent = node->self;
    else
        child->parent = left->self;

    this->node_write_and_pop(child);

    node->keys.erase(node->keys.begin(), node->keys.begin() + _min_order);
    node->children.erase(node->children.begin(), node->children.begin() + _min_order);

    return make_pair(left->keys.back(), node->keys.back());
}

void BplusTree::non_leaf_shift_element_insert(BplusNode* right, BplusNode* left, BplusNode* parent, int parent_left_index, int insert, bool from, key_t key, BplusNode* child) {
    if (!from) {
        right->keys.insert(right->keys.begin() + insert, key);
        right->children.insert(right->children.begin() + insert, child->self);

        left->keys.emplace_back(right->keys.front());
        left->children.emplace_back(right->children.front());
        if (insert != 0) {
            child->parent = right->self;
            this->child_node_write_and_pop(left, left->children.back());
        }
        else
            child->parent = left->self;

        this->node_write_and_pop(child);

        right->keys.erase(right->keys.begin());
        right->children.erase(right->children.begin());

        if (insert == right->keys.size()) {
            parent->keys[parent_left_index + 1] = right->keys.back();
            if (parent_left_index + 1 == parent->keys.size() - 1)
                this->parent_key_update(parent, parent->keys.back());
        }
    }
    else {
        left->keys.insert(left->keys.begin() + insert, key);
        left->children.insert(left->children.begin() + insert, child->self);

        right->keys.insert(right->keys.begin(), left->keys.back());
        right->children.insert(right->children.begin(), left->children.back());

        if (insert != left->keys.size() - 1) {
            child->parent = left->self;
            this->child_node_write_and_pop(right, right->children.front());
        }
        else
            child->parent = right->self;

        this->node_write_and_pop(child);

        left->keys.pop_back();
        left->children.pop_back();
    }

    parent->keys[parent_left_index] = left->keys.back();
}

// 简单的在非叶子结点插入，即存储未满的时候
void BplusTree::non_leaf_simple_insert(BplusNode* node, BplusNode* child, key_t key, int insert) {
    node->keys.insert(node->keys.begin() + insert, key);
    node->children.insert(node->children.begin() + insert, child->self);
    child->parent = node->self;
    this->node_write_and_pop(child);

    if (insert == node->keys.size() - 1)
        this->parent_key_update(node, key);
}

// 将非叶子结点插入，在存储满的情况下，分离结点，然后制造父母结点（或者将一个加入到另一个中），但是条件是要有child和key，node应该是已经在内存中
bool BplusTree::non_leaf_insert(BplusNode* node, BplusNode* child, key_t key) {
    int insert = key_binary_search_search(node, key);
    if (node->keys.size() == _max_order) {
        BplusNode* l_sib = this->node_insert_cache(node->prev);
        BplusNode* r_sib = this->node_insert_cache(node->next);
        BplusNode* parent = this->node_insert_cache(node->parent);

        int i = key_binary_search_search(parent, node->keys.front());
        BplusNode* sibling = nullptr;
        if (i != -1)
            sibling = sibling_less_select(l_sib, r_sib, parent, i) ? r_sib : l_sib;
        if (sibling != nullptr && sibling->keys.size() < _max_order) {
            if (sibling == l_sib) {
                this->non_leaf_shift_element_insert(node, l_sib, parent, i - 1, insert, false, key, child);
                cache_defer(r_sib);
                this->node_write_and_pop(l_sib);
            }
            else {
                this->non_leaf_shift_element_insert(r_sib, node, parent, i, insert, true, key, child);
                cache_defer(l_sib);
                this->node_write_and_pop(r_sib);
            }
            this->node_write_and_pop(node);
            this->node_write_and_pop(parent);
        }
        else {
            cache_defer(l_sib);
            cache_defer(r_sib);
            cache_defer(parent);
            std::pair<key_t, key_t> split_keys;
            BplusNode* sibling = non_leaf_new();
            split_keys = this->non_leaf_split(node, sibling, child, key, insert);
            return this->parent_node_build(sibling, node, split_keys.first, split_keys.second);
        }
    }
    else {
        this->non_leaf_simple_insert(node, child, key, insert);
        this->node_write_and_pop(node);
    }
    return true;
}

//当给叶结点增加元素，且结点满时，左增加结点，left是新创建的结点，没有在磁盘上写入
std::pair<key_t, key_t> BplusTree::leaf_split(BplusNode* leaf, BplusNode* left, key_t key, std::string datastring, int insert) {
    this->node_add(leaf, left);

    leaf->keys.insert(leaf->keys.begin() + insert, key);
    leaf->data.insert(leaf->data.begin() + insert, datastring);

    if (insert == leaf->keys.size() - 1)
        this->parent_key_update(leaf, key);

    for (int i = 0; i < _min_entry; i++) {
        left->keys.emplace_back(leaf->keys[i]);
        left->data.emplace_back(leaf->data[i]);
    }

    leaf->keys.erase(leaf->keys.begin(), leaf->keys.begin() + _min_entry);
    leaf->data.erase(leaf->data.begin(), leaf->data.begin() + _min_entry);

    return std::make_pair(left->keys.back(), leaf->keys.back());
}

void BplusTree::leaf_shift_element_insert(BplusNode* right, BplusNode* left, BplusNode* parent, int parent_left_index, int insert, bool from, key_t key, std::string datastring) {
    if (!from) {
        right->keys.insert(right->keys.begin() + insert, key);
        right->data.insert(right->data.begin() + insert, datastring);

        left->keys.emplace_back(right->keys.front());
        left->data.emplace_back(right->data.front());

        right->keys.erase(right->keys.begin());
        right->data.erase(right->data.begin());

        if (insert == right->keys.size()) {
            parent->keys[parent_left_index + 1] = right->keys.back();
            if (parent_left_index + 1 == parent->keys.size() - 1)
                this->parent_key_update(parent, parent->keys.back());
        }
    }
    else {
        left->keys.insert(left->keys.begin() + insert, key);
        left->data.insert(left->data.begin() + insert, datastring);

        right->keys.insert(right->keys.begin(), left->keys.back());
        right->data.insert(right->data.begin(), left->data.back());

        left->keys.pop_back();
        left->data.pop_back();
    }

    parent->keys[parent_left_index] = left->keys.back();
}

void BplusTree::leaf_simple_insert(BplusNode* leaf, key_t key, std::string datastring, int insert) {
    leaf->keys.insert(leaf->keys.begin() + insert, key);
    leaf->data.insert(leaf->data.begin() + insert, datastring);

    if (insert == leaf->keys.size() - 1)
        this->parent_key_update(leaf, key);
}

// 使用的时候注意leaf没有在内存中加入，也可以加入，并无太大影响
bool BplusTree::leaf_insert(BplusNode* leaf, key_t key, std::string datastring) {
    if (key_binary_search_remove(leaf, key) >= 0)
        return false;
    int insert = key_binary_search_search(leaf, key);

    for (int i = 0; i < MAX_CACHE_NUM; i++) {
        if (leaf == &used[i].second) {
            used[i].first = true;
            break;
        }
    }

    if (leaf->keys.size() == _max_entry) {
        BplusNode* l_sib = this->node_insert_cache(leaf->prev);
        BplusNode* r_sib = this->node_insert_cache(leaf->next);
        BplusNode* parent = this->node_insert_cache(leaf->parent);

        int i = key_binary_search_search(parent, leaf->keys.front());
        BplusNode* sibling = nullptr;
        if (i != -1)
            sibling = sibling_less_select(l_sib, r_sib, parent, i) ? r_sib : l_sib;
        if (sibling != nullptr && sibling->keys.size() < _max_entry) {
            if (sibling == l_sib) {
                this->leaf_shift_element_insert(leaf, l_sib, parent, i - 1, insert, false, key, datastring);
                cache_defer(r_sib);
                this->node_write_and_pop(l_sib);
            }
            else {
                this->leaf_shift_element_insert(r_sib, leaf, parent, i, insert, true, key, datastring);
                cache_defer(l_sib);
                this->node_write_and_pop(r_sib);
            }
            this->node_write_and_pop(leaf);
            this->node_write_and_pop(parent);
        }
        else {
            cache_defer(l_sib);
            cache_defer(r_sib);
            cache_defer(parent);
            std::pair<key_t, key_t> split_keys;
            BplusNode* sibling = leaf_new();
            split_keys = this->leaf_split(leaf, sibling, key, datastring, insert);
            return this->parent_node_build(sibling, leaf, split_keys.first, split_keys.second);
        }
    }
    else {
        this->leaf_simple_insert(leaf, key, datastring, insert);
        this->node_write_and_pop(leaf);
    }
    return true;
}

// 插入操作，给定key和data
bool BplusTree::BplusTree_insert(key_t key, std::string datastring) {
    this->mymutex.lock();
    BplusNode* node = this->node_seek(this->root);
    while (node != nullptr) {
        if (node->type == is_leaf)
            if (this->leaf_insert(node, key, datastring)) {
                this->mymutex.unlock();
                return true;
            }
            else {
                this->mymutex.unlock();
                return false;
            }
        else
            node = this->node_seek(node->children[key_binary_search_non_leaf_insert(node, key)]);
    }

    BplusNode* root = leaf_new();
    root->keys.emplace_back(key);
    root->data.emplace_back(datastring);
    this->root = this->new_node_append(root);
    this->level = 1;
    this->node_write_and_pop(root);
    this->mymutex.unlock();
    return true;
}

void BplusTree::non_leaf_shift_element_remove(BplusNode* right, BplusNode* left, BplusNode* parent, int parent_left_index, int remove, bool from) {
    if (!from) {
        right->keys.erase(right->keys.begin() + remove);
        right->children.erase(right->children.begin() + remove);

        right->keys.insert(right->keys.begin(), left->keys.back());
        right->children.insert(right->children.begin(), left->children.back());
        this->child_node_write_and_pop(right, right->children.front());

        left->keys.pop_back();
        left->children.pop_back();

        if (remove == right->keys.size() - 1) {
            parent->keys[parent_left_index + 1] = right->keys.back();
            if (parent_left_index + 1 == parent->keys.size() - 1)
                this->parent_key_update(parent, parent->keys.back());
        }
    }
    else {
        left->keys.erase(left->keys.begin() + remove);
        left->children.erase(left->children.begin() + remove);

        left->keys.emplace_back(right->keys.front());
        left->children.emplace_back(right->children.front());
        this->child_node_write_and_pop(left, left->children.back());

        right->keys.erase(right->keys.begin());
        right->children.erase(right->children.begin());
    }

    parent->keys[parent_left_index] = left->keys.back();
}

// 修改，如果from是true，删除的是左边的，如果是false，删除的是右边的
void BplusTree::non_leaf_merge(BplusNode* right, BplusNode* left, BplusNode* parent, int parent_right_index, int remove, bool from) {
    if (!from) {
        right->keys.erase(right->keys.begin() + remove);
        right->children.erase(right->children.begin() + remove);

        if (remove == right->keys.size()) {
            parent->keys[parent_right_index] = right->keys.back();
            if (parent_right_index == parent->keys.size() - 1)
                this->parent_key_update(parent, parent->keys.back());
        }

        for (int i = left->keys.size() - 1; i >= 0; i--) {
            right->keys.insert(right->keys.begin(), left->keys[i]);
            right->children.insert(right->children.begin(), left->children[i]);
            this->child_node_write_and_pop(right, left->children[i]);
        }
    }
    else {
        for (int i = left->keys.size() - 1; i >= 0; i--) {
            if (i != remove) {
                right->keys.insert(right->keys.begin(), left->keys[i]);
                right->children.insert(right->children.begin(), left->children[i]);
                this->child_node_write_and_pop(right, left->children[i]);
            }
        }
    }

    left->keys.clear();
    left->children.clear();
}

inline void BplusTree::non_leaf_simple_remove(BplusNode* node, int remove) {
    node->keys.erase(node->keys.begin() + remove);
    node->children.erase(node->children.begin() + remove);

    if (remove == node->keys.size())
        this->parent_key_update(node, node->keys.back());
}

void BplusTree::node_delete(BplusNode* node, BplusNode* left, BplusNode* right) {
    if (left != nullptr) {
        if (right != nullptr) {
            left->next = right->self;
            right->prev = left->self;
            this->node_write_and_pop(right);
        }
        else
            left->next = INVALID_OFFSET;
        this->node_write_and_pop(left);
    }
    else {
        if (right != nullptr) {
            right->prev = INVALID_OFFSET;
            this->node_write_and_pop(right);
        }
    }
    node->keys.clear();
    if (node->type == is_leaf)
        node->data.clear();
    else
        node->children.clear();

    node->type = free;
    if (this->free_blocks.empty()) {
        node->prev = INVALID_OFFSET;
        node->next = INVALID_OFFSET;
        node->parent = INVALID_OFFSET;
        this->node_write_and_pop(node);
    }
    else {
        BplusNode* left_freeblock = this->node_insert_cache(this->free_blocks.back());
        node->prev = left_freeblock->self;
        left_freeblock->next = node->self;
        this->node_write_and_pop(left_freeblock);
        node->next = INVALID_OFFSET;
        node->parent = INVALID_OFFSET;
        this->node_write_and_pop(node);
    }
    this->free_blocks.emplace_back(node->self);
}

void BplusTree::non_leaf_remove(BplusNode* node, int remove)
{
    if (node->parent == INVALID_OFFSET) {
        if (node->keys.size() == 2) {
            BplusNode* root = this->node_insert_cache(node->children.back());
            root->parent = INVALID_OFFSET;
            this->root = root->self;
            this->level--;
            this->node_delete(node, nullptr, nullptr);
            this->node_write_and_pop(root);
        }
        else {
            this->non_leaf_simple_remove(node, remove);
            this->node_write_and_pop(node);
        }
    }
    else if (node->keys.size() <= _min_order) {
        BplusNode* l_sib = this->node_insert_cache(node->prev);
        BplusNode* r_sib = this->node_insert_cache(node->next);
        BplusNode* parent = this->node_insert_cache(node->parent);

        int i = key_binary_search_search(parent, node->keys.front());
        BplusNode* sibling = nullptr;
        if (i != -1)
            sibling = sibling_more_select(l_sib, r_sib, parent, i) ? r_sib : l_sib;
        if (sibling != nullptr && sibling->keys.size() > _min_order) {
            if (sibling == l_sib) {
                this->non_leaf_shift_element_remove(node, l_sib, parent, i - 1, remove, false);
                cache_defer(r_sib);
                this->node_write_and_pop(l_sib);
            }
            else {
                this->non_leaf_shift_element_remove(r_sib, node, parent, i, remove, true);
                cache_defer(l_sib);
                this->node_write_and_pop(r_sib);
            }
            this->node_write_and_pop(node);
            this->node_write_and_pop(parent);
        }
        else {
            if (sibling == l_sib) {
                this->non_leaf_merge(node, l_sib, parent, i, remove, false);
                BplusNode* ll_sib = this->node_insert_cache(l_sib->prev);
                this->node_delete(l_sib, ll_sib, node);
                cache_defer(r_sib);
                this->non_leaf_remove(parent, i - 1);
            }
            else if (sibling == r_sib) {
                this->non_leaf_merge(r_sib, node, parent, i + 1, remove, true);
                this->node_delete(node, l_sib, r_sib);
                this->non_leaf_remove(parent, i);
            }
            else {
                std::cout << "no" << std::endl;
            }
        }
    }
    else {
        this->non_leaf_simple_remove(node, remove);
        this->node_write_and_pop(node);
    }
}

void BplusTree::leaf_shift_element_remove(BplusNode* right, BplusNode* left, BplusNode* parent, int parent_left_index, int remove, bool from) {
    if (!from) {
        right->keys.erase(right->keys.begin() + remove);
        right->data.erase(right->data.begin() + remove);

        right->keys.insert(right->keys.begin(), left->keys.back());
        right->data.insert(right->data.begin(), left->data.back());

        left->keys.pop_back();
        left->data.pop_back();

        if (remove == right->keys.size() - 1) {
            parent->keys[parent_left_index + 1] = right->keys.back();
            if (parent_left_index + 1 == parent->keys.size() - 1)
                this->parent_key_update(parent, parent->keys.back());
        }
    }
    else {
        left->keys.erase(left->keys.begin() + remove);
        left->data.erase(left->data.begin() + remove);

        left->keys.emplace_back(right->keys.front());
        left->data.emplace_back(right->data.front());

        right->keys.erase(right->keys.begin());
        right->data.erase(right->data.begin());
    }

    parent->keys[parent_left_index] = left->keys.back();
}

// 有点问题
void BplusTree::leaf_merge(BplusNode* right, BplusNode* left, BplusNode* parent, int parent_right_index, int remove, bool from) {
    if (!from) {
        right->keys.erase(right->keys.begin() + remove);
        right->data.erase(right->data.begin() + remove);

        if (remove == right->keys.size()) {
            parent->keys[parent_right_index] = right->keys.back();
            if (parent_right_index == parent->keys.size() - 1)
                this->parent_key_update(parent, parent->keys.back());
        }

        for (int i = left->keys.size() - 1; i >= 0; i--) {
            right->keys.insert(right->keys.begin(), left->keys[i]);
            right->data.insert(right->data.begin(), left->data[i]);
        }
    }
    else {
        for (int i = left->keys.size() - 1; i >= 0; i--) {
            if (i != remove) {
                right->keys.insert(right->keys.begin(), left->keys[i]);
                right->data.insert(right->data.begin(), left->data[i]);
            }
        }
    }

    left->keys.clear();
    left->data.clear();
}

inline void BplusTree::leaf_simple_remove(BplusNode* leaf, int remove) {
    leaf->keys.erase(leaf->keys.begin() + remove);
    leaf->data.erase(leaf->data.begin() + remove);

    if (remove == leaf->keys.size())
        this->parent_key_update(leaf, leaf->keys.back());
}

bool BplusTree::leaf_remove(BplusNode* leaf, key_t key) {
    int remove = key_binary_search_remove(leaf, key);
    if (remove < 0)
        return false;

    for (int i = 0; i < MAX_CACHE_NUM; i++) {
        if (leaf == &used[i].second) {
            used[i].first = true;
            break;
        }
    }

    if (leaf->parent == INVALID_OFFSET) {
        if (leaf->keys.size() == 1) {
            this->root = INVALID_OFFSET;
            this->level = 0;
            this->node_delete(leaf, nullptr, nullptr);
        }
        else {
            this->leaf_simple_remove(leaf, remove);
            this->node_write_and_pop(leaf);
        }
    }
    else if (leaf->keys.size() <= _min_entry) {
        BplusNode* l_sib = this->node_insert_cache(leaf->prev);
        BplusNode* r_sib = this->node_insert_cache(leaf->next);
        BplusNode* parent = this->node_insert_cache(leaf->parent);

        int i = key_binary_search_search(parent, leaf->keys.front());
        BplusNode* sibling = nullptr;
        if (i != -1)
            sibling = sibling_more_select(l_sib, r_sib, parent, i) ? r_sib : l_sib;
        if (sibling != nullptr && sibling->keys.size() > _min_entry) {
            if (sibling == l_sib) {
                this->leaf_shift_element_remove(leaf, l_sib, parent, i - 1, remove, false);
                this->node_write_and_pop(l_sib);
                cache_defer(r_sib);
            }
            else {
                this->leaf_shift_element_remove(r_sib, leaf, parent, i, remove, true);
                this->node_write_and_pop(r_sib);
                cache_defer(l_sib);
            }
            this->node_write_and_pop(leaf);
            this->node_write_and_pop(parent);
        }
        else {
            if (sibling == l_sib) {
                this->leaf_merge(leaf, l_sib, parent, i, remove, false);
                BplusNode* ll_sib = this->node_insert_cache(l_sib->prev);
                this->node_delete(l_sib, ll_sib, leaf);
                cache_defer(r_sib);
                this->non_leaf_remove(parent, i - 1);
            }
            else if (sibling == r_sib) {
                this->leaf_merge(r_sib, leaf, parent, i + 1, remove, true);
                this->node_delete(leaf, l_sib, r_sib);
                this->non_leaf_remove(parent, i);
            }
            else
                std::cout << "no" << std::endl;
        }
    }
    else {
        this->leaf_simple_remove(leaf, remove);
        this->node_write_and_pop(leaf);
    }
    return true;
}

bool BplusTree::BplusTree_delete(key_t key) {
    this->mymutex.lock();
    BplusNode* node = this->node_seek(this->root);
    if (node == nullptr || key > node->keys.back())
        return false;
    while (node->type == non_leaf)
        node = this->node_seek(node->children[key_binary_search_search(node, key)]);
    if (leaf_remove(node, key)) {
        this->mymutex.unlock();
        return true;
    }
    else {
        this->mymutex.unlock();
        return false;
    }
}

std::string BplusTree::BplusTree_search(key_t key) {
    this->mymutex.lock();
    BplusNode* node = this->node_seek(this->root);
    if (node == nullptr || key > node->keys.back())
        return "";
    while (node->type == non_leaf)
        node = this->node_seek(node->children[key_binary_search_search(node, key)]);
    int i = key_binary_search_remove(node, key);
    if (i < 0)
        return "";
    this->mymutex.unlock();
    return node->data[i];
}

std::string BplusTree::BplusTree_search_range(key_t key1, key_t key2) {
    this->mymutex.lock();
    std::string datastring;
    key_t min = key1 <= key2 ? key1 : key2;
    key_t max = min == key1 ? key2 : key1;

    BplusNode* node = this->node_seek(this->root);
    if (node == nullptr)
        return datastring;
    while (node->type == non_leaf)
        node = this->node_seek(node->children[key_binary_search_search(node, min)]);
    int i = key_binary_search_remove(node, min);
    if (i < 0)
        return datastring;
    while (node != nullptr && node->keys[i] <= max) {
        datastring = node->data[i]; // 这个后面可以增加输出或者返回什么
        if (++i >= node->keys.size()) {
            node = this->node_seek(node->next);
            i = 0;
        }
    }
    this->mymutex.unlock();
    return datastring;
}

BplusTree::BplusTree(std::string Filepath) {
    if (!this->BplusTree_Init(Filepath))
        std::cout << "BplusTree failed to build" << std::endl;
}

BplusTree::~BplusTree() {
    this->BplusTree_Deinit();
}

bool BplusTree::BplusTree_Init(std::string Filepath, int block_size) {
    if (Filepath.length() >= 1024) {
        std::cout << "Index file name too long!\n" << std::endl;
        return false;
    }

    if ((block_size & (block_size - 1)) % 2 != 0) {
        std::cout << "Block size must be pow of 2!\n" << std::endl;
        return false;
    }

    if (block_size <= (int)(sizeof(BplusNode) + sizeof(long) - sizeof(std::vector<off_t>) - sizeof(std::vector<key_t>) - sizeof(std::vector<std::string>))) {
        std::cout << "block size is too small for one node!\n" << std::endl;
        return false;
    }

    this->filepath = Filepath;

    char buffer[sizeof(int) * 2 + sizeof(off_t) * 3];
    long location = 0;

    ifstream file;
    file.open(Filepath, ios::binary | ios::in);
    if (file.fail())
        return false;
    if (file.peek() == EOF) {
        this->level = 0;
        this->file_size = sizeof(int) * 2 + sizeof(off_t) * 3;
        this->root = INVALID_OFFSET;
        this->free_blocks.clear();
        this->_block_size = block_size;
        file.close();
    }
    else {
        file.seekg(ios::beg);
        file.read(buffer, sizeof(int) * 2 + sizeof(off_t) * 3);
        file.close();

        memmove((char*)&this->level, buffer + location, sizeof(this->level));
        location += (int)sizeof(this->level);

        memmove((char*)&this->file_size, buffer + location, sizeof(this->file_size));
        location += (int)sizeof(this->file_size);

        memmove((char*)&this->root, buffer + location, sizeof(this->root));
        location += (int)sizeof(this->root);

        memmove(buffer + location, &this->_block_size, sizeof(this->_block_size));
        location += sizeof(this->_block_size);

        off_t first_freeblock;
        memmove((char*)&first_freeblock, buffer + location, sizeof(first_freeblock));
        location += (int)sizeof(first_freeblock);

        if (first_freeblock == INVALID_OFFSET)
            this->free_blocks.clear();
        else {
            this->free_blocks.emplace_back(first_freeblock);
            BplusNode* free_block = this->node_seek(first_freeblock);
            while (free_block->next != INVALID_OFFSET) {
                free_block = this->node_seek(free_block->next);
                this->free_blocks.emplace_back(free_block->self);
            }
        }
    }

    _max_order = (this->_block_size - sizeof(int) - sizeof(long) - 4 * sizeof(off_t)) / (sizeof(key_t) + sizeof(off_t));
    _min_order = ceil((double)_max_order / 2);
    _max_entry = (this->_block_size - sizeof(int) - sizeof(long) - 4 * sizeof(off_t)) / (sizeof(key_t) + sizeof(char[stringlength]));
    _min_entry = ceil((double)_max_entry / 2);

    _max_order = 3;
    _min_order = ceil((double)_max_order / 2);
    _max_entry = 3;
    _min_entry = ceil((double)_max_entry / 2);

    if (_max_order <= 2 || _max_entry <= 2) {
        std::cout << "block size is too small for one node!\n"
            << std::endl;
        return false;
    }

    for (int i = 0; i < MAX_CACHE_NUM; i++) {
        used[i].first = false;
    }

    return true;
}

void BplusTree::BplusTree_Deinit() {
    long location = 0;
    char buffer[sizeof(int) * 2 + sizeof(off_t) * 3];

    memmove(buffer + location, &this->level, sizeof(this->level));
    location += sizeof(this->level);

    memmove(buffer + location, &this->file_size, sizeof(this->file_size));
    location += sizeof(this->file_size);

    memmove(buffer + location, &this->root, sizeof(this->root));
    location += sizeof(this->root);

    memmove(buffer + location, &this->_block_size, sizeof(this->_block_size));
    location += sizeof(this->_block_size);

    off_t first_freeblock;
    if (this->free_blocks.empty())
        first_freeblock = INVALID_OFFSET;
    else
        first_freeblock = this->free_blocks.front();
    memmove(buffer + location, &first_freeblock, sizeof(first_freeblock));
    location += sizeof(first_freeblock);

    ofstream file;
    file.open(this->filepath, ios::binary | ios::out | ios::in);
    file.seekp(ios::beg);
    file.write(buffer, sizeof(int) * 2 + sizeof(off_t) * 3);
    file.close();

    _max_order = 0;
    _min_order = 0;
    _max_entry = 0;
    _min_entry = 0;

    this->free_blocks.clear();
}

static int key_binary_search_search(BplusNode* node, key_t target) {
    if (node == nullptr)
        return -1;

    if (node->keys.size() == 0)
        return -1;

    int low = -1;
    int high = node->keys.size();

    while (low + 1 < high) {
        int mid = low + (high - low) / 2;
        if (target <= node->keys[mid])
            high = mid;
        else
            low = mid;
    }
    return high;
}

static int key_binary_search_non_leaf_insert(BplusNode* node, key_t target) {
    int i = key_binary_search_search(node, target);
    return i == node->keys.size() ? --i : i;
}

static int key_binary_search_remove(BplusNode* node, key_t target) {
    if (node->keys.size() == 0)
        return -1;
    int low = 0;
    int high = node->keys.size() - 1;

    while (low <= high) {
        int mid = (low + high) / 2;
        if (target < node->keys[mid])
            high = mid - 1;
        else if (target > node->keys[mid])
            low = mid + 1;
        else
            return mid;
    }
    return -1;
}

// 在指定offset写this
void BplusTree::WriteDisk(BplusNode* node) {
    long location = 0;
    //char buffer[this->_block_size];
    char* buffer = new char[this->_block_size];

    memmove(buffer + location, &node->self, sizeof(node->self));
    location += sizeof(node->self);

    memmove(buffer + location, &node->parent, sizeof(node->parent));
    location += sizeof(node->parent);

    memmove(buffer + location, &node->prev, sizeof(node->prev));
    location += sizeof(node->prev);

    memmove(buffer + location, &node->next, sizeof(node->next));
    location += sizeof(node->next);

    memmove(buffer + location, &node->type, sizeof(node->type));
    location += sizeof(node->type);

    long numKeys = node->keys.size();
    memmove(buffer + location, &numKeys, sizeof(numKeys));
    location += sizeof(numKeys);

    if (node->type != free) {
        key_t keytemp;
        for (auto key : node->keys) {
            keytemp = key;
            memmove(buffer + location, &keytemp, sizeof(key));
            location += sizeof(key);
        }
    }

    if (node->type == non_leaf) {
        off_t childtemp;
        for (auto child : node->children) {
            childtemp = child;
            memmove(buffer + location, &childtemp, sizeof(child));
            location += sizeof(child);
        }
    }
    else if (node->type == is_leaf) {
        char str[stringlength];
        for (auto datastring : node->data) {
            strcpy(str, datastring.c_str());
            memmove(buffer + location, &str, sizeof(str));
            location += sizeof(str);
        }
    }

    ofstream file;
    file.open(this->filepath, ios::binary | ios::out | ios::in);
    file.seekp(node->self, ios::beg);
    file.write(buffer, this->_block_size);
    file.close();
}

//在指定位置读this
void BplusTree::ReadDisk(BplusNode* node, off_t offset) {
    long location = 0;
    //char buffer[this->_block_size];
    char* buffer = new char[this->_block_size];

    ifstream file;
    file.open(this->filepath, ios::binary | ios::in);
    file.seekg(offset, ios::beg);
    file.read(buffer, this->_block_size);
    file.close();

    memmove((char*)&node->self, buffer + location, sizeof(node->self));
    location += sizeof(node->self);

    memmove((char*)&node->parent, buffer + location, sizeof(node->parent));
    location += sizeof(node->parent);

    memmove((char*)&node->prev, buffer + location, sizeof(node->prev));
    location += sizeof(node->prev);

    memmove((char*)&node->next, buffer + location, sizeof(node->next));
    location += sizeof(node->next);

    memmove((char*)&node->type, buffer + location, sizeof(node->type));
    location += sizeof(node->type);

    long numKeys;
    memmove((char*)&numKeys, buffer + location, sizeof(numKeys));
    location += sizeof(numKeys); // 将基本数据读入，类似头文件

    node->keys.clear(); // 清空
    key_t key;
    for (int i = 0; i < numKeys; ++i) {
        memmove((char*)&key, buffer + location, sizeof(key));
        location += sizeof(key);
        node->keys.emplace_back(key); // 将关键值读入
    }

    if (node->type == non_leaf) {
        node->children.clear();
        off_t child;
        for (int i = 0; i < numKeys; ++i) {
            memmove((char*)&child, buffer + location, sizeof(child));
            location += sizeof(child);
            node->children.emplace_back(child);
        } // 非叶子结点，就将children读入
    }
    else if (node->type == is_leaf) {
        node->data.clear();
        char str[stringlength];
        for (int i = 0; i < numKeys; ++i) {
            memmove(str, buffer + location, sizeof(str));
            location += sizeof(str);
            std::string datastring = str;
            node->data.emplace_back(datastring);
        } // 叶子结点，就将data指针读入
    }
}

//int main() {
//    BplusTree tree("D:\\task\\Bplustree_storage\\Bplus.txt");
//    return 0;
//}