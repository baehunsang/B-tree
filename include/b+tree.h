#ifndef B_PLUS_TREE
#define B_PLUS_TREE
#include <bits/stdc++.h>
using namespace std;

#define SPLIT_POS(n) ((n)&1 ? (n)/2 + 1 : (n)/2)

template <typename T, typename U>
class BPlusTree {
public:
    // children[i] is lower then keys[i]
    // children[i + 1] is equal or greater then keys[i]
    // (pointers[i], key[i]) is pair, the pair exist max n - 1
    // pointers max size is n

    struct Node {
        bool isLeaf;
        vector<T> keys;
        vector<Node*> pointers;
        
        // leaf node
        // in leaf node, records, keys len is equal
        vector<U> records;
        Node* next;

        Node* parent;
        Node(bool leaf = false):
            isLeaf(leaf),
            next(nullptr),
            parent(nullptr) {}
    };

    Node* root;
    // Minimum degree (defines the range for the number of
    // keys)
    int n;

    void insert_in_leaf(Node* L, T K, U P);
    void insert_in_parent(Node* N, T K_tmp, Node* N_tmp);

public:
    BPlusTree(int degree): root(nullptr), n(degree){}

    void insert(T K, U P);
    U find_key(T v);

};

template <typename T, typename U>
void BPlusTree<T, U>::insert(T K, U P){
    if(!root){
        root = new Node(true);
        root->keys.push_back(K);
        root->records.push_back(P);
    }
    else{
        Node* leaf = root;
        while(!leaf->isLeaf){
            // K < leaf->keys[idx], idx < leaf->keys.size()
            int idx = upper_bound(leaf->keys.begin(), leaf->keys.end(), K) - leaf->keys.begin();
            leaf = leaf->pointers[idx];
        }
        if(leaf->keys.size() < n - 1){
            insert_in_leaf(leaf, K, P);
        }
        else{
            //leaf split
            Node* L_tmp = new Node(true);

            Node Tmp;
            Tmp.keys.insert(Tmp.keys.begin(), leaf->keys.begin(), leaf->keys.end());
            Tmp.records.insert(Tmp.records.begin(), leaf->records.begin(), leaf->records.end());
            
            insert_in_leaf(&Tmp, K, P);
            
            L_tmp->next = leaf->next;
            leaf->next = L_tmp;

            leaf->keys.resize(0);
            leaf->records.resize(0);
            
            leaf->keys.insert(leaf->keys.begin(), Tmp.keys.begin(), (Tmp.keys.begin() + SPLIT_POS(n)));
            leaf->records.insert(leaf->records.begin(), Tmp.records.begin(), (Tmp.records.begin() + SPLIT_POS(n)));

            L_tmp->keys.insert(L_tmp->keys.begin(), (Tmp.keys.begin() + SPLIT_POS(n)), Tmp.keys.end());
            L_tmp->records.insert(L_tmp->records.begin(), (Tmp.records.begin() + SPLIT_POS(n)), Tmp.records.end());

            T K_tmp = L_tmp->keys.front();
            insert_in_parent(leaf, K_tmp, L_tmp);
        }
    }
}

template <typename T, typename U>
void BPlusTree<T, U>::insert_in_leaf(Node* L, T K, U P){
    if(K < L->keys.front()){
        L->keys.insert(L->keys.begin(), K);
        L->records.insert(L->records.begin(), P);
    }
    else{
        int idx = 0;
        while(idx < L->keys.size() && K >= L->keys[idx]){
            idx++;
        }
        
        L->keys.insert(L->keys.begin() + idx, K);
        L->records.insert(L->records.begin() + idx, P);
    }
}

template <typename T, typename U>
void BPlusTree<T, U>::insert_in_parent(Node* N, T K_tmp, Node* N_tmp){
    if(!N->parent){
        Node* R = new Node(false);
        N->parent = R;
        N_tmp->parent = R;
        R->keys.push_back(K_tmp);
        R->pointers.push_back(N);
        R->pointers.push_back(N_tmp);
        root = R;
        return;
    }
    Node* P = N->parent;
    if(P->pointers.size() < n){
        auto pos = find(P->pointers.begin(), P->pointers.end(), N);
        int idx = pos - P->pointers.begin();
        P->pointers.insert(pos + 1, N_tmp);
        P->keys.insert(P->keys.begin() + idx, K_tmp);
        
        for(auto it = P->pointers.begin();it != P->pointers.end();it++){
            (*it)->parent = P;
        }
    }
    else{
        // split
        Node Tmp;
        Tmp.isLeaf = false;

        Tmp.keys.insert(Tmp.keys.begin(), P->keys.begin(), P->keys.end());
        Tmp.pointers.insert(Tmp.pointers.begin(), P->pointers.begin(), P->pointers.end());

        auto pos = find(Tmp.pointers.begin(), Tmp.pointers.end(), N);
        int idx = pos - Tmp.pointers.begin();
        Tmp.pointers.insert(pos + 1, N_tmp);
        Tmp.keys.insert(Tmp.keys.begin() + idx, K_tmp);

        P->keys.resize(0);
        P->pointers.resize(0);

        Node* P_tmp = new Node(false);

        // Copy T.P1…T.P⌈(n+1)∕2⌉ into P
        // note (P, K) is one pair
        P->keys.insert(P->keys.begin(), Tmp.keys.begin(), (Tmp.keys.begin() + SPLIT_POS(n + 1) - 1));
        P->pointers.insert(P->pointers.begin(), Tmp.pointers.begin(), (Tmp.pointers.begin() + SPLIT_POS(n + 1)));
        
        for(auto it = P->pointers.begin();it != P->pointers.end();it++){
            (*it)->parent = P;
        }
        T K_tmp2 = Tmp.keys[SPLIT_POS(n + 1) - 1];

        // Copy T.P⌈(n+1)∕2⌉+1 …T.Pn+1
        P_tmp->keys.insert(P_tmp->keys.begin(), (Tmp.keys.begin() + SPLIT_POS(n + 1)), Tmp.keys.end());
        P_tmp->pointers.insert(P_tmp->pointers.begin(), (Tmp.pointers.begin() + SPLIT_POS(n + 1)), Tmp.pointers.end());
        
        for(auto it = P_tmp->pointers.begin();it != P_tmp->pointers.end();it++){
            (*it)->parent = P_tmp;
        }
        insert_in_parent(P, K_tmp2, P_tmp);
    }

}

template <typename T, typename U>
U BPlusTree<T, U>::find_key(T v){
    Node* C = root;
    while(!C->isLeaf){
        int idx =C->keys.size() - 1;
        while(idx >= 0 && v <= C->keys[idx]){
            idx--;
        }
        idx++;
        if(idx == C->keys.size()){
            C = C->pointers.back(); 
        }
        else if(v == C->keys[idx]) C = C->pointers[idx + 1];
        else C = C->pointers[idx];
    }
    auto seek = find(C->keys.begin(), C->keys.end(), v);
    if(seek != C->keys.end()){
        return C->records[seek - C->keys.begin()]; 
    }
    else return nullptr;
}

#endif
