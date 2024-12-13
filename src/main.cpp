#include <iostream>
#include <stdlib.h>
#include "b+tree.h"

using namespace std;

int main(){
    string records[13] = {
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
        "Kim",
        "Adams"
    };

    BPlusTree<string, string*> tree(4);
    for(int i=0; i< 13; i++){
        tree.insert(records[i], &records[i]);
    }
    for(int i=0; i< 13; i++){
        assert(tree.find_key(records[i]) == &records[i]);
    }
    tree.print_all_record();
    printf("[-] insertion TC pass\n");

    tree.delete_key("Srinivasan");
    assert(tree.find_key("Srinivasan") == nullptr);
    tree.delete_key("Singh");
    tree.delete_key("Wu");
    assert(tree.find_key("Singh") == nullptr);
    assert(tree.find_key("Wu") == nullptr);
    for(int i=0; i< 13; i++){
        tree.delete_key(records[i]);
        assert(tree.find_key(records[i]) == nullptr);
    }

    printf("[-] deletion TC pass\n");
    return 0;
}