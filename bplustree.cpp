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
        newNode->next->prev = newNode;
        this->parent->children.push_back(newNode);
        //Move half pairs of this node to new node.
        for (int i=0; i<=mid; ++i) {
            newNode->data.push_back(this->data.back());
            this->data.pop_back();
        }
        newNode->sortData();
        this->parent->keys.push_back(newNode->data.front()->key);
        if (this->parent->keys.size() == this->degree) {
            this->parent->split();
        }
        this->parent->sortKeys();
        this->parent->sortChildren();
    }
    //Split an internal node.
    else {
        //Set new node.
        newNode->isLeaf = false;
        newNode->prev = this;
        newNode->next = this->next;
        this->next = newNode;
        newNode->next->prev = newNode;
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
        if (this->parent->keys.size() == this->degree) {
            this->parent->split();
        }
        this->keys.pop_back();
        this->parent->sortKeys();
        this->parent->sortChildren();
    }
    return newNode;
}

void BPlusTreeNode::lBorrow() {
    //Leaf borrowing from left sibling.
    if (this->isLeaf) {
        //Borrow a pair.
        this->data.push_front(this->prev->data.back());
        this->prev->data.pop_back();
        //Renew key in parent node.
        this->parent->keys.front() = this->data.front()->key;
    }
    //Int node borrowing from left sibling
    else {
        //Take a key from parent.
        this->keys.push_front(this->parent->keys.back());
        this->parent->keys.pop_back();
        //Parent take a key from left sibling.
        this->parent->keys.push_front(this->prev->keys.back());
        this->prev->keys.pop_back();
        //Move child accordingly.
        this->children.push_front(this->prev->children.back());
        this->prev->children.pop_back();
    }
}

void BPlusTreeNode::rBorrow() {
    //Leaf borrowing from right sibling.
    if (this->isLeaf) {
        //Borrow a pair from right(next).
        this->data.push_back(this->next->data.front());
        this->next->data.pop_front();
        //Renew key in parent node.
        this->next->parent->keys.front() = this->next->data.front()->key;
    }
    //Int node borrowing from right sibling.
    else {
        this->keys.push_back(this->parent->keys.front());
        this->parent->keys.pop_front();
        this->parent->keys.push_back(this->next->keys.front());
        this->next->keys.pop_front();
        this->children.push_back(this->next->children.front());
        this->next->children.pop_back();
    }

}

void BPlusTreeNode::lMerge() {
    //Leaf merging: combine data and delete in-between key in parent.
    if (this->isLeaf) {
        this->prev->data.insert(this->prev->data.end(), this->data.begin(), this->data.end());
        this->parent->del(this->data.front()->key);
    }
    //Int node merging.
    else {

    }
    //Maintain prev & next.
    this->prev->next = this->next;
    this->next->prev = this->prev;

    free(this);
}

void BPlusTreeNode::rMerge() {
    //Leaf merging: combine data and delete in-between key in parent.
    if (this->isLeaf) {
        this->data.insert(this->data.end(), this->next->data.begin(), this->next->data.end());
        this->parent->del(this->next->data.front()->key);
    }
    //Int node merging.
    else {

    }
    //Maintain prev & next.
    this->next->next->prev = this;
    this->next = this->next->next;

    free(this->next);
}

void BPlusTreeNode::del(int key) {
    int lowerBound = (this->degree - 1) / 2 + 1;
    if (this->isLeaf) {
        auto delPair = this->search(key);
        for (auto iter = this->data.begin(); iter != this->data.end(); ++iter) {
            if (*iter == delPair) {
                this->data.erase(iter);
            }
        }
        //Deficient. Try to borrow first, del higher level later.
        if (this->data.size() < lowerBound) {
            //If left is a sibling and has extra pair.
            if ((this->prev->parent == this->parent) && (this->prev->data.size() > lowerBound)) {
                this->lBorrow();
            }
            //If right is a sibling and has extra pair.
            else if ((this->next->parent == this->parent) && (this->next->data.size() > lowerBound)) {
                this->rBorrow();
            }

            else {

            }
        }

    }
    else {

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
    //Start deleting from leaf and do a bottom-up maintenance.
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
