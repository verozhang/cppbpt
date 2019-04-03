//
// Created by verozhang on 4/1/19.
//

#ifndef BPT_BPLUSTREE_H
#define BPT_BPLUSTREE_H
#include <vector>
#include <algorithm>
using namespace std;

class Pair{
public:
    int key;
    float value;

    Pair();
    ~Pair();
    bool operator<(Pair*);
};

class BPlusTreeNode{
public:
    BPlusTreeNode* parent;
    bool isLeaf;
    int degree;
    BPlusTreeNode* prev;
    BPlusTreeNode* next;
    vector<int> keys;
    vector<BPlusTreeNode*> children;
    vector<Pair*> data;

    BPlusTreeNode();
    ~BPlusTreeNode();
    bool operator<(BPlusTreeNode*);
    BPlusTreeNode* findLeaf(int);
    Pair* search(int);
    void insert(Pair*);
    BPlusTreeNode* split();
    void sortKeys();
    void sortChildren();
    void sortData();
};

class BPlusTree{
public:
    BPlusTreeNode* root;
    int degree;

    BPlusTree();
    ~BPlusTree();
    Pair* search(int);
    void insert(Pair*);
};

#endif //BPT_BPLUSTREE_H
