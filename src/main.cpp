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

    cout << "\n";

    printf("[-] insertion TC pass\n");
    for(int i=0; i< 13; i++){
        tree.delete_key(records[i]);
        assert(tree.find_key(records[i]) == nullptr);
        cout << "After delete '" << records[i] <<"'"<<endl;
        tree.print_all_record();
        cout<< "\n\n";
    }

    printf("[-] deletion TC pass\n");
    return 0;
}