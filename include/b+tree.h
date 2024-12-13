#ifndef B_PLUS_TREE
#define B_PLUS_TREE
#include <bits/stdc++.h>
using namespace std;

#define SPLIT_POS(n) ((n)&1 ? (n)/2 + 1 : (n)/2)

template <typename T, typename U>
class BPlusTree {
private:
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
    // n
    int n;

    void insert_in_leaf(Node* L, T K, U P);
    void insert_in_parent(Node* N, T K_tmp, Node* N_tmp);
    void delete_entry(Node* N, T K, Node* P);
    Node* find_leaf(T K);

public:
    BPlusTree(int degree): root(nullptr), n(degree){}

    void insert(T K, U P);
    U find_key(T v);
    void print_all_record();
    void delete_key(T K);

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
        
        for(auto child : P->pointers){
            child->parent = P;
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
        
        for(auto child : P->pointers){
            child->parent = P;
        }
        T K_tmp2 = Tmp.keys[SPLIT_POS(n + 1) - 1];

        // Copy T.P⌈(n+1)∕2⌉+1 …T.Pn+1
        P_tmp->keys.insert(P_tmp->keys.begin(), (Tmp.keys.begin() + SPLIT_POS(n + 1)), Tmp.keys.end());
        P_tmp->pointers.insert(P_tmp->pointers.begin(), (Tmp.pointers.begin() + SPLIT_POS(n + 1)), Tmp.pointers.end());
        
        for(auto child : P_tmp->pointers){
            child->parent = P_tmp;
        }
        insert_in_parent(P, K_tmp2, P_tmp);
    }

}

template <typename T, typename U>
U BPlusTree<T, U>::find_key(T v){
    Node* C = find_leaf(v);
    if(!C) return nullptr;
    
    auto seek = find(C->keys.begin(), C->keys.end(), v);
    if(seek != C->keys.end()){
        return C->records[seek - C->keys.begin()]; 
    }
    else return nullptr;
}

template <typename T, typename U>
void BPlusTree<T, U>::print_all_record(){
    Node* C = root;
    while (C &&!C->isLeaf){
        C = C->pointers.front();
    }
    while(C){
        for(int i=0; i< C->keys.size(); i++){
            cout << "record: " << C->keys[i] << ", location: "<< hex << C->records[i] << endl; 
        }
        C = C->next;
    }
    
}

template <typename T, typename U>
typename BPlusTree<T, U>::Node* BPlusTree<T, U>::find_leaf(T K){
    Node* C = root;
    while(C && !C->isLeaf){
        int idx =C->keys.size() - 1;
        while(idx >= 0 && K <= C->keys[idx]){
            idx--;
        }
        idx++;
        if(idx == C->keys.size()){
            C = C->pointers.back(); 
        }
        else if(K == C->keys[idx]) C = C->pointers[idx + 1];
        else C = C->pointers[idx];
    }
    return C;
}

template <typename T, typename U>
void BPlusTree<T, U>::delete_key(T K){
    if(find_key(K)){
        Node* L = find_leaf(K);
        delete_entry(L, K, nullptr);
    }
}

template <typename T, typename U>
void BPlusTree<T, U>::delete_entry(Node* N, T K, Node* P){
    auto pos = find(N->keys.begin(), N->keys.end(), K);
    auto pos2 = find(N->pointers.begin(), N->pointers.end(), P);
    int idx = pos - N->keys.begin();
    N->keys.erase(pos);
    if(N->isLeaf){
        N->records.erase(N->records.begin() + idx);
    }
    else{
        N->pointers.erase(pos2);
    }
    
    if(N->isLeaf && N == root && N->records.size() == 0){
        root = nullptr;
        delete N;
        return;
    }

    else if(!N->parent && N->pointers.size() == 1){
        root = N->pointers.front();
        root->parent = nullptr;
        delete N;
        return;
    }
    else if(N->parent){
        // SPLIT_POS(n - 1) -> underbound : "number of (P,K)"
        if(N->keys.size() < SPLIT_POS(n - 1)){
            Node* Pr = N->parent;
            Node* N_tmp = nullptr;
            T K_tmp;
            auto pos = find(Pr->pointers.begin(), Pr->pointers.end(), N);
            int idx = pos - Pr->pointers.begin();

            // default : left sibling
            // but if N is front of P->pointers then right sibling
            if(!idx){
                N_tmp = Pr->pointers[idx + 1];
                K_tmp = Pr->keys[idx];
            }
            else{
                N_tmp = Pr->pointers[idx - 1];
                K_tmp = Pr->keys[idx - 1];
            }

            // merge
            if(N->keys.size() + N_tmp->keys.size() <= n - 1
            && N->pointers.size() + N_tmp->pointers.size() <= n
            ){
                // N predecessor
                if(!idx){
                    swap(N, N_tmp);
                }
                if(!N->isLeaf){
                    N_tmp->keys.push_back(K_tmp);
                    N_tmp->keys.insert(N_tmp->keys.end(), N->keys.begin(), N->keys.end());
                    N_tmp->pointers.insert(N_tmp->pointers.end(), N->pointers.begin(), N->pointers.end());
                    for(auto ch : N_tmp->pointers){
                        ch->parent = N_tmp;
                    }
                }
                else{
                    N_tmp->keys.insert(N_tmp->keys.end(), N->keys.begin(), N->keys.end());
                    N_tmp->records.insert(N_tmp->records.end(), N->records.begin(), N->records.end());
                    N_tmp->next = N->next;
                }
                delete_entry(Pr, K_tmp, N);
                delete N;
            }

            //redistribution
            else{
                // N_tmp is predecessor (left sibling)
                if(idx != 0){
                    if(!N->isLeaf){
                        Node* P_m = N_tmp->pointers.back();
                        T K_to_replace = N_tmp->keys.back();
                        N_tmp->pointers.pop_back();
                        N_tmp->keys.pop_back();
                        N->pointers.insert(N->pointers.begin(), P_m);
                        N->keys.insert(N->keys.begin(), K_tmp);
                        Pr->keys[idx - 1] = K_to_replace;
                        for(auto ch : N->pointers){
                            ch->parent = N;
                        }
                    }
                    else{
                        U P_m = N_tmp->records.back();
                        T K_m = N_tmp->keys.back();
                        N_tmp->records.pop_back();
                        N_tmp->keys.pop_back();
                        N->records.insert(N->records.begin(), P_m);
                        N->keys.insert(N->keys.begin(), K_m);
                        Pr->keys[idx - 1] = K_m;
                    }
                }

                // N is predecessor
                else {
                    if(!N->isLeaf){
                        Node* P_m = N_tmp->pointers.front();
                        T K_to_replace = N_tmp->keys.front();
                        N_tmp->pointers.erase(N_tmp->pointers.begin());
                        N_tmp->keys.erase(N_tmp->keys.begin());
                        N->pointers.insert(N->pointers.end(), P_m);
                        N->keys.insert(N->keys.end(), K_tmp);
                        Pr->keys[idx] = K_to_replace;
                        for(auto ch : N->pointers){
                            ch->parent = N;
                        }
                    }
                    else{
                        U P_m = N_tmp->records.front();
                        T K_m = N_tmp->keys.front();
                        N_tmp->records.erase(N_tmp->records.begin());
                        N_tmp->keys.erase(N_tmp->keys.begin());
                        N->records.insert(N->records.end(), P_m);
                        N->keys.insert(N->keys.end(), K_m);
                        Pr->keys[idx] = K_m;
                    }
                }
            }
        }
    }
}


#endif
