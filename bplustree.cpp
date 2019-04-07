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
    newNode->tree = this->tree;
    newNode->parent = this->parent;
    newNode->degree = this->degree;
    //Split a leaf.
    if (this->isLeaf) {
        //Set new node.
        newNode->isLeaf = true;
        newNode->prev = this;
        newNode->next = this->next;
        this->next = newNode;
        if (newNode->next != nullptr) {
            newNode->next->prev = newNode;
        }
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
        if (newNode->next != nullptr) {
            newNode->next->prev = newNode;
        }
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
        this->children.back()->parent = newNode;
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

bool BPlusTreeNode::isDeficient() {
    //Root can have 2 children.
    if (this->tree->root == this) {
        //Leaf root means only 1 node. Never deficient.
        if (this->isLeaf) {
            return false;
        }
        //Int root is deficient when only having 1 child.
        else {
            return (this->children.size() < 2);
        }
    }
    else {
        //Leaf deficient when
        if (this->isLeaf) {
            return this->data.size() < ((this->degree + 1) / 2 - 1);
        }
        else {
            return this->children.size() < ((this->degree + 1) / 2 - 1);
        }
    }
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
        this->prev->next = this->next;
        if (this->next != nullptr) {
            this->next->prev = this->prev;
        }
        free(this);
    }
    //Int node merging.
    else {
        //Find corresponding in-between key.
        int findKey = 0;
        for (int i=0; i<this->parent->keys.size(); ++i) {
            if (this->parent->keys[i] > this->prev->keys.back()) {
                findKey = this->parent->keys[i];
                this->prev->keys.push_back(findKey);
                break;
            }
        }
        this->prev->keys.insert(this->prev->keys.end(), this->keys.begin(), this->keys.end());
        this->prev->children.insert(this->prev->children.end(), this->children.begin(), this->children.end());
        this->parent->del(findKey);
        this->prev->next = this->next;
        if (this->next != nullptr) {
            this->next->prev = this->prev;
        }
        free(this);
    }
}

void BPlusTreeNode::rMerge() {
    //Leaf merging: combine data and delete in-between key in parent.
    if (this->isLeaf) {
        this->next->data.insert(this->next->data.begin(), this->data.begin(), this->data.end());
        this->parent->del(this->next->data.front()->key);
        this->next->next->prev = this;
        this->next = this->next->next;
        free(this);
    }
    //Int node merging.
    else {
        //Find corresponding in-between key.
        int findKey = 0;
        for (int i=0; i<this->parent->keys.size(); ++i) {
            if (this->parent->keys[i] > this->next->keys.front()) {
                findKey = this->parent->keys[i-1];
                this->next->keys.push_front((findKey));
            }
        }
        this->next->keys.insert(this->next->keys.begin(), this->keys.begin(), this->keys.end());
        this->next->children.insert(this->next->children.begin(), this->children.begin(), this->children.end());
        this->parent->del(findKey);
        this->next->prev = this->prev;
        if (this->prev != nullptr) {
            this->prev->next = this->next;
        }
        free(this);
    }
}

void BPlusTreeNode::rootDel() {
    this->tree->root = this->children.front();
    free(this);
}

void BPlusTreeNode::del(int key) {
    int lowerBound = (this->degree - 1) / 2;
    //Delete in a leaf.
    if (this->isLeaf) {
        for (int i=0; i<this->data.size(); ++i) {
            if (this->data[i]->key == key) {
                this->data.erase(this->data.begin() + i);
                break;
            }
        }
        //Deficient. Try to borrow first, del higher level later.
        if (this->isDeficient()) {
            //If left is a sibling and has extra pair.
            if ((this->prev != nullptr) && (this->prev->parent == this->parent) && (this->prev->data.size() > lowerBound)) {
                this->lBorrow();
            }
            //If right is a sibling and has extra pair.
            else if ((this->next != nullptr) && (this->next->parent == this->parent) && (this->next->data.size() > lowerBound)) {
                this->rBorrow();
            }
            //Try to merge with left. If left exists and is sibling.
            else if ((this->prev != nullptr) && (this->prev->parent == this->parent)) {
                this->lMerge();
            }
            //Try to merge with right
            else if ((this->next != nullptr) && (this->next->parent == this->parent)){
                this->rMerge();
            }
            //Deficient root (2-level tree only).
            else if (this->parent != this->tree->root){
                this->rootDel();
            }
            //Delete parent to make it deficient as well.
            else {
                this->parent->del(key);
            }
        }
    }
    //Delete in int node. Called only when children merging.
    else {
        for (int i=0; i<this->keys.size(); ++i) {
            if (this->keys[i] == key) {
                this->keys.erase(this->keys.begin() + i);
                break;
            }
        }
        //Deficient.
        if (this->isDeficient()) {
            if ((this->prev != nullptr) && (this->prev->parent == this->parent) && (this->prev->data.size() > lowerBound)) {
                this->lBorrow();
            }
                //If right is a sibling and has extra pair.
            else if ((this->next != nullptr) && (this->next->parent == this->parent) && (this->next->data.size() > lowerBound)) {
                this->rBorrow();
            }
                //Try to merge with left. If left exists and is sibling.
            else if ((this->prev != nullptr) && (this->prev->parent == this->parent)) {
                this->lMerge();
            }
                //Try to merge with right
            else if ((this->next != nullptr) && (this->next->parent == this->parent)){
                this->rMerge();
            }
            else if (this->parent == nullptr){
                this->rootDel();
            }
            else {
                this->parent->del(key);
            }
        }
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
    this->root->tree = this;
    this->degree = 3;
    this->root->degree = this->degree;
}

BPlusTree::~BPlusTree() = default;

Pair* BPlusTree::search(int key) {
    return this->root->search(key);
}

deque<Pair*>* BPlusTree::rangeSearch(int left, int right) {
    BPlusTreeNode* lLeaf = this->root->findLeaf(left);
    BPlusTreeNode* rLeaf = this->root->findLeaf(right);
    auto result = new(deque<Pair*>);
    return result;
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
    newRoot->tree = this;
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
