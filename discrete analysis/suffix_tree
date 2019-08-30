#include <iostream>
#include <map>
#include <vector>
#include <queue>

struct TNode {
	TNode(int l = 0, int r = 0, int p = -1) {
		left = l;
		right = r;
		parent = p;
		link = -1;
		depth = -1;
	}
	int Length() {  
		return right - left; 
	}
	int &Get(char c) {
		if (!next.count(c)) {  
			next[c] = -1;
		}
		return next[c];
	}
	int left;
	int right;
	int parent;
	int link;
	int depth;
	std::map <char,int> next;
};

struct TCurrentState {
	int v, pos;
	TCurrentState(int vv, int ppos) {
		v = vv;
		pos = ppos;
	}
};

std::string pattern;
std::vector<TNode> suff_tree;
int suff_tree_size, pattern_size;
TCurrentState pointer(0, 0);

TCurrentState GetState(TCurrentState tcs, int left, int right) {
	while (left < right) {
		if (tcs.pos == suff_tree[tcs.v].Length()) {
			tcs = TCurrentState(suff_tree[tcs.v].Get(pattern[left]), 0);
			if (tcs.v == -1) { 
				return tcs;
			}
		}
		else {
			if (pattern[suff_tree[tcs.v].left + tcs.pos] != pattern[left]) {
				return TCurrentState(-1, -1);
			}
			if (right - left < suff_tree[tcs.v].Length() - tcs.pos) {
				return TCurrentState(tcs.v, tcs.pos + right-left);
			}
			left += suff_tree[tcs.v].Length() - tcs.pos;
			tcs.pos = suff_tree[tcs.v].Length();
		}
	}
	return tcs;
}

int Split(TCurrentState tcs) {
	if (tcs.pos == suff_tree[tcs.v].Length()) {
		return tcs.v;
	}
	if (tcs.pos == 0) {
		return suff_tree[tcs.v].parent;
	}
	TNode tn = suff_tree[tcs.v];
	int id = suff_tree_size++;
	suff_tree[id] = TNode(tn.left, tn.left + tcs.pos, tn.parent);
	suff_tree[tn.parent].Get(pattern[tn.left]) = id;
	suff_tree[id].Get(pattern[tn.left + tcs.pos]) = tcs.v;
	suff_tree[tcs.v].parent = id;
	suff_tree[tcs.v].left += tcs.pos;
	return id;
}

int GetLink(int val) {
	if (suff_tree[val].link != -1) { 
		return suff_tree[val].link;
	}
	if (suff_tree[val].parent == -1) { 
		return 0;
	}
	int to = GetLink(suff_tree[val].parent);
	return suff_tree[val].link = Split(GetState(TCurrentState(to, suff_tree[to].Length()), suff_tree[val].left + (suff_tree[val].parent == 0), suff_tree[val].right));
}

void ContinueSuffTree(int pos) {
	for(;;) {
		TCurrentState tpointer = GetState(pointer, pos, pos + 1);
		if (tpointer.v != -1) {
			pointer = tpointer;
			return;
		}
		int middle = Split(pointer);
		int leaf = suff_tree_size++;
		suff_tree[leaf] = TNode(pos, pattern_size, middle);
		suff_tree[middle].Get(pattern[pos]) = leaf;
		pointer.v = GetLink(middle);
		pointer.pos = suff_tree[pointer.v].Length();
		if (!middle) { 
			break;
		}
	}
}
 
void CreateSuffTree() {
	suff_tree_size = 1;
	for (int i = 0; i < pattern_size; i++) {
        ContinueSuffTree(i);
    }
}

void GetDepths() {
	suff_tree[0].depth = 0;
	std::queue < int > nodes;
	for (auto i: suff_tree[0].next) {
		nodes.push(i.second);
	}
	while (!nodes.empty()) {
		int c = nodes.front();
		nodes.pop();
		suff_tree[c].depth = suff_tree[suff_tree[c].parent].depth + suff_tree[c].right - suff_tree[c].left;
		for (auto i: suff_tree[c].next) {
			nodes.push(i.second);
		}
	}
}

void Jump(TNode*& nod, TNode*& nod_orig, std::vector <int>& match_stat, std::string& text, std::string& pattern, int& jump, int& pos) {
	if (jump == 0) {
		while (nod->parent != -1) {
			if (nod->next.count(text[pos] > 0)) {
				nod = &suff_tree[nod->next.count(text[pos])];
				break;	
			}
			else {
				match_stat.push_back(nod->depth);
				nod = &suff_tree[nod->link];
			}
		}
		if (nod->parent == -1) {
			while (nod->next.count(text[pos]) == 0 && pos < text.size()) {
				match_stat.push_back(0);
				pos++;
			}
			if (pos < text.size()) {
				nod = &suff_tree[nod->next[text[pos]]];
			}
		}
	}
	else {
		int bounce;
		if (nod_orig->parent == 0) {
			bounce = nod_orig->left + 1;
		}
		else {
			bounce = nod_orig->left;
		}
		nod = &suff_tree[nod->next[pattern[bounce]]];
		bounce += nod->right - nod->left;
		while (jump > nod->right - nod->left && bounce < pattern.size()) {
			jump -= nod->right - nod->left;
			nod = &suff_tree[nod->next[pattern[bounce]]];
			bounce += nod->right - nod->left;
		}
		if (jump == nod->right - nod->left) {
			if (nod->next.count(text[pos]) > 0) {
				nod = &suff_tree[nod->next[text[pos]]];
				jump = 0;
			}
		}
	}
}

int main() {
	std::cin >> pattern;
	pattern_size = pattern.size();
	suff_tree.resize(2 * pattern_size);
	CreateSuffTree();
	GetDepths();
	std::string text;
	std::cin >> text;
	std::vector < int > match_stat;
	int pos = 0, jump = 0;
	TNode* nod = &suff_tree[pos];
	while (pos < text.size()) {
		if (nod->next.count(text[pos]) == 0) {
			match_stat.push_back(0);
			pos++;
		}
		else {
			nod = &suff_tree[nod->next[text[pos]]];
			break;
		}
	}
	for (;;) {
		if (match_stat.size() == text.size()) {
			break;
		}
		if (jump == nod->right - nod->left) {
			match_stat.push_back(nod->depth);
			TNode* nod_orig = nod;
			if (nod->parent == 0) {
				jump--;
				nod = &suff_tree[nod->parent];
			}
			else {
				nod = &suff_tree[suff_tree[nod->parent].link];
			}
			Jump(nod, nod_orig, match_stat, text, pattern, jump, pos);
		}
		else {
			for (int i = nod->left + jump; i < nod->right; i++) {
				if (text[pos] == pattern[i] && pos != text.size()) {			
					if (i == nod->right - 1) {
						if (pos != text.size() - 1 && nod->next.count(text[pos + 1]) > 0) {
							nod = &suff_tree[nod->next[text[pos + 1]]];
							jump = 0;
							pos++;
							break;
						}
						else {
							match_stat.push_back(nod->depth - (nod->right - i) + 1);
							TNode* nod_orig = nod;
							if (nod->parent == 0) {
								jump = nod->right - nod->left - 1;
								nod = &suff_tree[nod->parent];
							}
							else {
								jump = nod->right - nod->left;
								nod = &suff_tree[suff_tree[nod->parent].link];
							}
							pos++;
							Jump(nod, nod_orig, match_stat, text, pattern, jump, pos);
							break;
						}
					}
					pos++;
				}
				else {
					match_stat.push_back(nod->depth - (nod->right - i));
					TNode* nod_orig = nod;
					if (nod->parent == 0) {
						jump = i - nod->left - 1;
						nod = &suff_tree[nod->parent];
					}
					else {
						jump = i - nod->left;
						nod = &suff_tree[suff_tree[nod->parent].link];
					}
					Jump(nod, nod_orig, match_stat, text, pattern, jump, pos);
					break;
				}
			}
		}
	}
	for (int i = 0; i < match_stat.size(); i++) {
		if (match_stat[i] == pattern.length()) {
			std::cout << i + 1 << std::endl;
		}
	}
}
