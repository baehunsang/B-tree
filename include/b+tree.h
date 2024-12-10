#ifndef B_PLUS_TREE
#define B_PLUS_TREE
#include <bits/stdc++.h>
using namespace std;

template <typename T>
class BPlusTree {
public:
    struct Node {
        bool isLeaf;
        vector<T> keys;
        vector<Node*> children;
        Node* next;
    };
};
#endif
