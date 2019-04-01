#ifndef BPT_BPLUSTREE_H
#define BPT_BPLUSTREE_H

#include <vector>
#include <algorithm>
using namespace std;

class Data{
public:
    int key;
    float value;

    Data(int, float);
    ~Data();
};

class BPlusTreeNode{
public:
    bool isLeaf;
    int degree;

};

class IntNode: public BPlusTreeNode{
public:
    IntNode* parent;
    vector<int> keys;
    vector<BPlusTreeNode> children;

    IntNode();
    ~IntNode();
    Data* search(int);
    void insert(int, BPlusTreeNode*);
    IntNode* split();
};

class LeafNode: public BPlusTreeNode{
public:
    IntNode* parent;
    LeafNode* prev;
    LeafNode* next;
    vector<int> keys;
    vector<Data> children;

    LeafNode();
    ~LeafNode();
    Data* search(int);
    void insert(int, Data*);
    LeafNode* split();

};

class BPlusTree{
public:
    int degree;
    BPlusTreeNode* root;

    BPlusTree(int);
    ~BPlusTree();
    Data* search(int);
    LeafNode* findNode(int);
    void insert(Data*);
};


#endif //BPT_BPLUSTREE_H