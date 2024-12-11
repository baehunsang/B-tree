#include <iostream>
#include <stdlib.h>
#include "b+tree.h"

using namespace std;

int main(){
    string records[12] = {
        "Srinivasan",
        "Wu",
        "Mozart",
        "Einstein",
        "El Said",
        "Gold",
        "Katz",
        "Califieri",
        "Singh",
        "Crick",
        "Brandt",
        "Kim"
    };

    BPlusTree<string, string*> tree(4);
    for(int i=0; i< 12; i++){
        tree.insert(records[i], &records[i]);
    }
    for(int i=0; i< 12; i++){
        assert(tree.find_key(records[i]) == &records[i]);
    }
    tree.print_all_record();
    printf("[-] TC pass\n");

    return 0;
}