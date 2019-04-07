//
// Created by verozhang on 4/1/19.
//

#ifndef BPT_BPLUSTREE_H
#define BPT_BPLUSTREE_H
#include <deque>
#include <algorithm>
using namespace std;

class BPlusTree;
class Pair{
public:
    int key;
    float value;

    Pair();
    ~Pair();
};

class BPlusTreeNode{
public:
    BPlusTree* tree;
    BPlusTreeNode* parent;
    bool isLeaf;
    int degree;
    BPlusTreeNode* prev;
    BPlusTreeNode* next;
    deque<int> keys;
    deque<BPlusTreeNode*> children;
    deque<Pair*> data;

    BPlusTreeNode();
    ~BPlusTreeNode();
    BPlusTreeNode* findLeaf(int);
    Pair* search(int);
    void insert(Pair*);
    BPlusTreeNode* split();
    bool isDeficient();
    void lBorrow();
    void rBorrow();
    void lMerge();
    void rMerge();
    void rootDel();
    void del(int);
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
    deque<Pair*>* rangeSearch(int, int);
    void insert(Pair*);
    void del(int);
    void grow();//Height +1.
    void shrink();//Height -1.
};

bool cmpPairs(Pair*, Pair*);
bool cmpNodes(BPlusTreeNode*, BPlusTreeNode*);

#endif //BPT_BPLUSTREE_H
