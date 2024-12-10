#include <iostream>
#include <stdlib.h>
#include "b+tree.h"

using namespace std;

int main(){
    int records[7] = {1,2,3,4,5,6,7}; 
    for(int i=0; i< 8; i++){
        records[i] = i + 1;
    }
    BPlusTree<int, int*> tree(3);
    // Insert elements
    tree.insert(records[0], &records[0]);
    tree.insert(records[1], &records[1]);
    tree.insert(records[2], &records[2]);
    tree.insert(records[3], &records[3]);
    tree.insert(records[4], &records[4]);
    tree.insert(records[5], &records[5]);
    tree.insert(records[6], &records[6]);
    tree.insert(records[7], &records[7]);


    //cout << "B+ Tree after insertions:" << endl;
    //tree.printTree();
//
    //// Search for a key
    //int searchKey = 15;
    //cout << "\nSearching for key " << searchKey << ": "
    //     << (tree.search(searchKey) ? "Found" : "Not Found")
    //     << endl;
//
    //// Perform a range query
    //int lower = 10, upper = 25;
    //vector<int> rangeResult = tree.rangeQuery(lower, upper);
    //cout << "\nRange query [" << lower << ", " << upper
    //     << "]: ";
    //for (int key : rangeResult) {
    //    cout << key << " ";
    //}
    //cout << endl;
//
    //// Remove a key
    //int removeKey = 20;
    //tree.remove(removeKey);
    //cout << "\nB+ Tree after removing " << removeKey << ":"
    //     << endl;
    //tree.printTree();

    return 0;
}