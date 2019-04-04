//
// Created by verozhang on 4/1/19.
//

#include "bplustree.h"

Pair::Pair() {
    this->key = 0;
    this->value = 0;
}
Pair::~Pair() = default;

BPlusTreeNode::BPlusTreeNode() {
    this->parent = nullptr;
    this->isLeaf = true;
    this->degree = 3;
    this->prev = nullptr;
    this->next = nullptr;
};
BPlusTreeNode::~BPlusTreeNode() = default;

//Given a key, find which leaf this key should be in.
BPlusTreeNode* BPlusTreeNode::findLeaf(int key) {
    //Already a leaf.
    if (this->isLeaf) {
        return this;
    }
    //Not a leaf. Search in the
    else {
        for (int i=0; i<this->keys.size(); ++i) {
            if (key < this->keys[i]) {
                return this->children[i]->findLeaf(key);
            }
        }
        return this->children.back()->findLeaf(key);
    }
}

//Given a key, search for it by returning a pair.
Pair* BPlusTreeNode::search(int key) {
    //Searching in a leaf: search in data and return pointer to pair.
    if (this->isLeaf) {
        for (int i=0; i<this->data.size(); ++i) {
            if (this->data[i]->key ==  key) {
                return this->data[i];
            }
        }
        //Not found.
        return nullptr;
    }
    //Searching in a internal node: search in children and do deeper search in corresponding child.
    else {
        return this->findLeaf(key)->search(key);
    }
}

//Insert a pair into a node (the subtree rooted at node).
void BPlusTreeNode::insert(Pair* newPair) {
    //Inserting in a leaf.
    if (this->isLeaf) {
        this->data.push_back(newPair);
        this->sortData();
        if (this->data.size() == this->degree) {
            this->split();
        }
    }
    //Inserting in a internal node: search for the correct leaf to insert.
    else {
        this->findLeaf(newPair->key)->insert(newPair);
        if (this->children.front()->children.size() == this->degree + 1) {
            this->children.front()->split();
        }
    }
}

//Split a node into two. Called when inserting.
BPlusTreeNode* BPlusTreeNode::split() {
    int mid = this->degree / 2;
    auto newNode = new(BPlusTreeNode);
    newNode->parent = this->parent;
    newNode->degree = this->degree;
    //Split a leaf.
    if (this->isLeaf) {
        //Set new node.
        newNode->isLeaf = true;
        newNode->prev = this;
        newNode->next = this->next;
        this->next = newNode;
        this->parent->children.push_back(newNode);
        //Move half pairs of this node to new node.
        for (int i=0; i<=mid; ++i) {
            newNode->data.push_back(this->data.back());
            this->data.pop_back();
        }
        newNode->sortData();
        this->parent->keys.push_back(newNode->data.front()->key);
        this->parent->sortKeys();
        this->parent->sortChildren();
    }
    //Split an internal node.
    else {
        //Set new node.
        newNode->isLeaf = false;
        this->parent->children.push_back(newNode);
        //Move half keys and children to new node;
        for (int i=0; i<mid; ++i) {
            newNode->keys.push_back(this->keys.back());
            this->children.back()->parent = newNode;
            newNode->children.push_back(this->children.back());
            this->keys.pop_back();
            this->children.pop_back();
        }
        //Always 1 more child than key.
        newNode->children.push_back(this->children.back());
        this->children.pop_back();
        newNode->sortChildren();
        newNode->sortKeys();
        //Move mid key to higher level.
        this->parent->keys.push_back(this->keys.back());
        this->keys.pop_back();
        this->parent->sortKeys();
        this->parent->sortChildren();
    }
    return newNode;
}

void BPlusTreeNode::del(int) {
    //Deleting the 1st pair in

    auto iter = this->data.begin();


    while (iter != this->data.end()) {

    }
}

//Compare 2 nodes with smallest pair in each. Called in BPlusTreeNode::sortChildren
bool cmpNodes(BPlusTreeNode* a, BPlusTreeNode* b) {
    //Search down to first leaf.
    while (!a->isLeaf) {
        a = a->children.front();
        b = b->children.front();
    }
    //Compare key of first pair.
    return (a->data.front()->key < b->data.front()->key);

}

void del(int key) {

}

//Sort a int node's key list.
void BPlusTreeNode::sortKeys() {
    sort(this->keys.begin(), this->keys.end());
}

//Sort a int node's child list.
void BPlusTreeNode::sortChildren() {
    sort(this->children.begin(), this->children.end(), cmpNodes);
}

//Compare 2 pairs with key. Called in BPlusTreeNode::sortData.
bool cmpPairs(Pair* a, Pair* b) {
    return (a->key < b->key);
}

//Sort a leaf node's data list.
void BPlusTreeNode::sortData() {
    sort(this->data.begin(), this->data.end(), cmpPairs);
}

BPlusTree::BPlusTree() {
    this->root = new(BPlusTreeNode);
    this->root->isLeaf = true;
    this->degree = 3;
    this->root->degree = this->degree;
}

BPlusTree::~BPlusTree() = default;

Pair* BPlusTree::search(int key) {
    return this->root->search(key);
}

void BPlusTree::insert(Pair* newPair) {
    this->grow();
    this->root->insert(newPair);
    if (this->root->keys.empty()) {
        this->shrink();
    }
}

void BPlusTree::del(int key) {
    auto targetLeaf = this->root->findLeaf(key);
    auto targetPair = targetLeaf->search(key);
    //Not found.
    if (targetPair == nullptr) {
        return;
    }
    else {
        targetLeaf->del(key);
    }
}

void BPlusTree::grow() {
    auto newRoot = new(BPlusTreeNode);
    newRoot->degree = this->degree;
    newRoot->isLeaf = false;
    newRoot->children.push_back(this->root);
    this->root->parent = newRoot;
    this->root = newRoot;
}

void BPlusTree::shrink() {
    BPlusTreeNode* tmp = this->root;
    this->root = this->root->children.front();
    free(tmp);
}
