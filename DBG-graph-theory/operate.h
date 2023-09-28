#pragma once
#include"public.h"
#include "operate.h"
Node* FindORCreateNode(std::string s_id, std::vector<Node*>& Nvector, std::map<std::string, unsigned>& maps);
Node* link(std::string pointer, Node* bodys, std::map<std::string, unsigned>& maps, std::vector<Node*>& Nvector);
bool vectorFind(std::vector<Node*> v, Node* f);

void SubgraphSegmentation(std::vector<Node*>&bridge, std::vector<Node*>Nvector, std::vector<Node*>&HeadNvector, std::vector<Node*>&EndNvector);