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
    void grow();//Height +1.
    void shrink();//Height -1.

};

bool cmpPairs(Pair*, Pair*);
bool cmpNodes(BPlusTreeNode*, BPlusTreeNode*);

#endif //BPT_BPLUSTREE_H
