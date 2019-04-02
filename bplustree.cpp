//
// Created by verozhang on 4/1/19.
//

#include "bplustree.h"

BPlusTreeNode::BPlusTreeNode() {}
BPlusTreeNode::~BPlusTreeNode() {}

BPlusTreeNode* BPlusTreeNode::findLeaf(int key) {
    //Already a leaf.
    if (this->isLeaf) {
        return this;
    }
    //Not a leaf. Search in the
    else {
        for (int i=0; i<this->keys.size(); ++i) {
            if (this->keys[i] > key) {
                return this->children[i]->findLeaf(key);
            }
        }
        return this->children.back()->findLeaf(key);
    }
}

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

void BPlusTreeNode::insert(Pair* newPair) {
    //Inserting in a leaf.
    if (this->isLeaf) {
        //1.Empty data. Used only when initializing.
        //2.Larger than largest key.
        if (this->data.empty() or this->data.back()->key <= newPair->key) {
            this->data.push_back(newPair);
        }
        else{
            int i = 0;
            while (i < this->data.size()) {
                if (this->data[i]->key > newPair->key) {
                    this->data.insert(this->data.begin() + i, newPair);
                    break;
                }
                else {
                    ++i;
                }
            }
        }
        if (this->data.size() == this->degree) {
            this->split();
        }
    }
    //Inserting in a internal node: search for the correct leaf to insert.
    else {
        this->findLeaf(newPair->key)->insert(newPair);
    }
}

BPlusTreeNode* BPlusTreeNode::split() {
    int mid = this->degree / 2;
    auto newNode = new(BPlusTreeNode);
    newNode->parent = this->parent;
    newNode->degree = this->degree;
    //Split a leaf
    if (this->isLeaf) {
        //Set new node.
        newNode->isLeaf = true;
        newNode->prev = this;
        newNode->next = this->next;
        this->next = newNode;
        this->parent->children.push_back(newNode);
        //Move half pairs of this node to new node.
        for (int i=0; i<mid; ++i) {
            newNode->data.push_back(this->data.back());
            this->data.pop_back();
        }
    }
    //Split an internal node.
    else {

    }
    return newNode;
}

BPlusTree::BPlusTree() {
    this->root = new(BPlusTreeNode);
    this->root->isLeaf = true;
    this->degree = 3;
    this->root->degree = this->degree;
}

void BPlusTree::insert(Pair* newPair) {
    //If root is a leaf and has (m-1) children, create new root, and set current root as its child.
    if (this->root->isLeaf && this->root->data.size() == degree - 1) {
        auto newNode = new(BPlusTreeNode);
        newNode->isLeaf = false;
        newNode->degree = this->degree;
        newNode->children.push_back(this->root);
        this->root = newNode;
    }
    //Start searching from root.
    return this->root->insert(newPair);
}