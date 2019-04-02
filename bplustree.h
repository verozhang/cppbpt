//
// Created by verozhang on 4/1/19.
//

#ifndef BPT_BPLUSTREE_H
#define BPT_BPLUSTREE_H
#include <vector>
using namespace std;

struct Pair{
    int key;
    float value;
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
};

class BPlusTree{
public:
    BPlusTreeNode* root;
    int degree;

    BPlusTree();
    ~BPlusTree();
    BPlusTreeNode* search(int);
    void insert(Pair*);
};

#endif //BPT_BPLUSTREE_H
