# B+ tree implementation in C++
---

## 1. motivation
---

I studied B+ tree implementation in C++ form [GeeksforGeeks](https://www.geeksforgeeks.org/cpp-program-to-implement-b-plus-tree/) but its implementation is not B+ tree. (In B+ tree, all (record location, key) pair need to be located in leaf node.)

So I implemented B+ tree using the prototype of `GeeksforGeeks` implementation.


```c++
template <typename T, typename U>
    struct Node {
        bool isLeaf;
        vector<T> keys;
        vector<Node*> pointers;
        
        // leaf node
        // in leaf node, records, keys len is equal
        vector<U> records;
        Node* next;

        Node* parent; // parent is added
        Node(bool leaf = false):
            isLeaf(leaf),
            next(nullptr),
            parent(nullptr) {}
    };
```

```
include/b+tree.h -> actual implementation of B+ tree
src/main.cpp -> test code

```

## 2. build
---

```shell
mkdir build
cd build
cmake ..
make
```

## 3. result
---

```
==189877== Memcheck, a memory error detector
==189877== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==189877== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==189877== Command: ./b+tree
==189877==
record: Adams, location: 0x1ffefffde0
record: Brandt, location: 0x1ffefffda0
record: Califieri, location: 0x1ffefffd40
record: Crick, location: 0x1ffefffd80
record: Einstein, location: 0x1ffefffcc0
record: El Said, location: 0x1ffefffce0
record: Gold, location: 0x1ffefffd00
record: Katz, location: 0x1ffefffd20
record: Kim, location: 0x1ffefffdc0
record: Mozart, location: 0x1ffefffca0
record: Singh, location: 0x1ffefffd60
record: Srinivasan, location: 0x1ffefffc60
record: Wu, location: 0x1ffefffc80
[-] insertion TC pass
[-] deletion TC pass
==189877==
==189877== HEAP SUMMARY:
==189877==     in use at exit: 0 bytes in 0 blocks
==189877==   total heap usage: 68 allocs, 68 frees, 78,320 bytes allocated
==189877==
==189877== All heap blocks were freed -- no leaks are possible
==189877==
==189877== For lists of detected and suppressed errors, rerun with: -s
==189877== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
