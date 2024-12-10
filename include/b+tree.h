#ifndef B_PLUS_TREE
#define B_PLUS_TREE
#include <bits/stdc++.h>
using namespace std;

template <typename T, typename U>
class BPlusTree {
public:
    // children[i] is lower then keys[i]
    // children[i + 1] is equal or greater then keys[i]
    struct Node {
        bool isLeaf;
        vector<T> keys;
        vector<Node*> children;
        // vector for records 
        vector<U> records;
        Node* next;

        Node(bool leaf = false):
            isLeaf(leaf),
            next(nullptr) {}
    };

    Node* root;
    // Minimum degree (defines the range for the number of
    // keys)
    int t;

    /* Function to split a child node
    This function is called when child's room is full
    `n = 2*t` , room size : `[(n - 1) / 2]` ~ `(n - 1)` 
    `parent` : parent of splited child node (affected)
    `child` : original child node
    return none, this function manage pointer of 2 args */
    void splitChild(Node* parent, int index, Node* child);

    // Function to insert a key in a non-full node
    void insertNonFull(Node* node, T key, U record);

    // Function to remove a key from a node
    void remove(Node* node, T key);

    // Function to borrow a key from the previous sibling
    void borrowFromPrev(Node* node, int index);

    // Function to borrow a key from the next sibling
    void borrowFromNext(Node* node, int index);

    // Function to merge two nodes
    void merge(Node* node, int index);

    // Function to print the tree
    void printTree(Node* node, int level);

public:
    BPlusTree(int degree): root(nullptr), t(degree){}

    void insert(T key, U record);
    bool search(T key);
    void remove(T key);
    vector<T> rangeQuery(T lower, T upper);
    void printTree();
};

template <typename T, typename U>
void BPlusTree<T, U>::splitChild(Node* parent, int index, Node* child){
    // right part
    Node* newChild = new Node(child->isLeaf);
    
    // Add empty child node
    parent->children.insert(parent->children.begin() + index + 1, newChild);

    // insert the middle of child( child->keys[t - 1] ) to parent 
    parent->keys.insert(parent->keys.begin() + index, child->keys[t - 1]);

    // assign bigger part to newChild
    // must include middle of original child -> to find child node ptr.
    newChild->keys.assign(child->keys.begin() + (t - 1), child->keys.end());

    // resize
    child->keys.resize(t - 1);

    // split non-leaf node
    if(!child->isLeaf){
        newChild->children.assign(child->children.begin() + (t - 1), child->children.end());

        
        child->children.resize(t - 1);
    }
    else{
        // linked list insertion
        newChild->next = child->next;
        child->next = newChild;
        newChild->records.assign(child->records.begin() + (t - 1), child->records.end());
        child->records.resize(t - 1);
    }
}

template <typename T, typename U>
void BPlusTree<T, U>::insertNonFull(Node* node, T key, U record){
    if(node->isLeaf){
        // upper bound returns the location greater then key
        auto insert_pos = upper_bound(node->keys.begin(), node->keys.end(), key);
        int idx = insert_pos - node->keys.begin();
        node->keys.insert(insert_pos, key);
        node->records.insert(node->records.begin() + idx, record);
    }

    else {
        int i = node->keys.size() - 1;
        while(i >= 0 && key < node->keys[i]){
            i--;
        }
        i++;

        // child room is full
        if (node->children[i]->keys.size() == 2*t - 1){
            splitChild(node, i, node->children[i]);
            if(key > node->keys[i]){
                i++;
            }
        }
        insertNonFull(node->children[i], key, record);
    }
}

template <typename T, typename U>
void BPlusTree<T, U>::insert(T key, U record){
    if(root == nullptr){
        root = new Node(true);
        root->keys.push_back(key);
        root->records.push_back(record);
    }
    else{
        // root is full
        // n == 2*t
        if(root->keys.size() == 2*t - 1){
            Node* newRoot = new Node();
            newRoot->children.push_back(root);
            splitChild(newRoot, 0, root);
            root = newRoot;
        }
        insertNonFull(root, key, record);
    }
}
#endif
